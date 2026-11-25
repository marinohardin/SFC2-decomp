"""The incremental build and the score cache (harness/lib/cache.py)."""
import json
import os
import stat

import pytest

from harness.lib import cache, coff, paths, source, target, verify


@pytest.fixture
def cached(tmp_path, monkeypatch):
    monkeypatch.delenv('HARNESS_NO_CACHE', raising=False)
    monkeypatch.setattr(paths, 'SHARED', str(tmp_path / 'shared'))
    monkeypatch.setattr(cache, '_memo', {})
    return tmp_path


# Dependencies ----------------------------------------------------------------------------------

def tree(root, files):
    for name, text in files.items():
        p = root / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)


def test_dependencies_follow_every_include_beside_and_on_the_path(tmp_path):
    tree(tmp_path, {
        'src/u.cpp': '#include "a.h"\n#include <b.h>\n#include <windows.h>\n',
        'src/a.h': '#if 0\n#include "c.h"\n#endif\n#include "a.h"\n',
        'src/c.h': '',
        'inc/b.h': '  #  include "d.h"\n',
        'inc/d.h': '',
    })
    deps = cache.dependencies(str(tmp_path / 'src/u.cpp'), [str(tmp_path / 'inc')])
    want = ['inc/b.h', 'inc/d.h', 'src/a.h', 'src/c.h', 'src/u.cpp']
    assert deps == sorted(os.path.realpath(tmp_path / w) for w in want)


def test_a_macro_include_cannot_be_keyed(tmp_path):
    tree(tmp_path, {'u.cpp': '#include "a.h"\n', 'a.h': '#define H "b.h"\n#include H\n'})
    assert cache.dependencies(str(tmp_path / 'u.cpp'), []) is None


def test_include_dirs_come_from_the_command_and_msvc_extra_include():
    cmd = ['cl', '/I', '/x/op', '/I/x/port', '/nologo', '-I', 'rel']
    assert cache.include_dirs(cmd, {'MSVC_EXTRA_INCLUDE': 'Z:/x/stl;/y'}) == \
        ['/x/op', '/x/port', 'rel', '/x/stl', '/y']


# Incremental build ------------------------------------------------------------------------------

FAKE_CL = '''#!/bin/sh
# Writes <unit>.obj and <unit>.asm into the /Fo folder and counts its calls.
echo x >> "$CALLS"
out=; for a in "$@"; do case $a in /Fo*) out=${a#/Fo} ;; esac; unit=$a; done
[ -n "$FAIL" ] && { echo "$unit(1) : error C2065"; exit 2; }
b=${unit%.cpp}
printf 'obj %s' "$(cat "$unit" ./*.h 2>/dev/null)" > "$out$b.obj"
echo listing > "$out$b.asm"
echo "$unit"
'''


@pytest.fixture
def unit(cached, monkeypatch):
    src = cached / 'op'
    tree(src, {'u.cpp': '#include "u.h"\nint f() { return 1; }\n', 'u.h': 'int f();\n',
               'other.h': ''})
    cl = cached / 'cl'
    cl.write_text(FAKE_CL)
    cl.chmod(cl.stat().st_mode | stat.S_IXUSR)
    monkeypatch.setattr(paths, 'CL', str(cl))
    monkeypatch.setenv('CALLS', str(cached / 'calls'))

    def build(profile=None):
        return source.compile_unit(str(src / 'u.cpp'), profile or {'flags': '/O1'}, str(src),
                                   build_root=str(cached / 'build'))

    def calls():
        return len((cached / 'calls').read_text().split())
    return src, build, calls


def test_an_unchanged_unit_is_not_rebuilt(unit):
    src, build, calls = unit
    first = build()
    assert first['ok'] and not first.get('cached') and calls() == 1
    again = build()
    assert again['ok'] and again['cached'] and calls() == 1
    assert (again['obj'], again['listing'], again['log']) == \
        (first['obj'], first['listing'], first['log'])


def test_a_changed_header_flag_or_unrelated_file(unit):
    src, build, calls = unit
    build()
    (src / 'u.h').write_text('int f(); // changed\n')
    assert not build().get('cached') and calls() == 2
    assert not build({'flags': '/O2'}).get('cached') and calls() == 3
    (src / 'other.h').write_text('// not included\n')
    assert build({'flags': '/O2'}).get('cached') and calls() == 3


def test_no_cache_and_failed_builds_always_compile(unit, monkeypatch):
    src, build, calls = unit
    build()
    monkeypatch.setenv('HARNESS_NO_CACHE', '1')
    assert not build().get('cached') and calls() == 2
    monkeypatch.delenv('HARNESS_NO_CACHE')
    monkeypatch.setenv('FAIL', '1')
    assert not build()['ok']
    monkeypatch.delenv('FAIL')
    assert not build().get('cached') and calls() == 4
    assert build().get('cached') and calls() == 4


# Score cache ------------------------------------------------------------------------------------

class Fn:
    address, name, size, mangled = 0x00401000, 'f', 4, '_f'


class Reloc:
    def as_dict(self):
        return {'offset': 1}


@pytest.fixture
def scoring(cached, monkeypatch):
    calls = []

    def fake_build(address, size, obj, sym, exe, index):
        calls.append(sym)
        return dict(target=b'\xc3' * 4, base=b'\xc3' * 4, object=b'', relocs=[Reloc()],
                    strict=True, first_diff=None, rewritten=0)

    def fake_objdiff(t, b, sym, out_dir):
        ins = [{'diff_kind': 'DIFF_REPLACE', 'instruction': {'address': 0, 'formatted': 'ret'}}]
        return {'match_percent': 75.0, 'instructions': ins}, {'instructions': ins}, ''
    monkeypatch.setattr(target, 'build', fake_build)
    monkeypatch.setattr(verify, 'objdiff', fake_objdiff)

    def score(code=b'\xc3' * 4, stamp=0, rel='u.cpp', fn=Fn):
        raw = bytearray(coff.write_object(code, '_f', []))
        raw[4:8] = stamp.to_bytes(4, 'little')
        out = cached / 'build' / f'u{len(calls)}-{stamp}'
        out.mkdir(parents=True)
        (out / 'u.obj').write_bytes(bytes(raw))
        b = dict(obj=str(out / 'u.obj'), listing=None, out_dir=str(out))
        unit = dict(rel=rel, path=rel, file=rel, line=1, other_lines=[])
        return verify.score_function(fn(), unit, b, None, None, symbol_hint='_f')
    return score, calls


def test_an_unchanged_object_is_not_compared_again(scoring):
    score, calls = scoring
    first = score()
    again = score(stamp=1234)        # the same code, recompiled: only the timestamp differs
    assert calls == ['_f']
    for k in ('score', 'exact', 'size', 'relocs', 'strict', 'diff_rows', 'diff_count', 'error'):
        assert again[k] == first[k], k
    assert again['diff_rows'] == [('+0x0', 'ret', 'ret', 'replace')]
    with open(again['side_by_side']) as f, open(first['side_by_side']) as g:
        assert f.read() == g.read()
    assert json.load(open(os.path.join(os.path.dirname(again['side_by_side']),
                                       'result.json')))['score'] == 75.0


def test_a_hit_keeps_the_unit_and_name_of_the_function_scored_now(scoring):
    """A function moved to another file or renamed, with the same object: the score is reused,
    the unit and the name are this run's."""
    class Renamed(Fn):
        name = 'tFoo::f'
    score, calls = scoring
    score()
    moved = score(stamp=1, rel='unsorted/tFoo.cpp', fn=Renamed)
    assert calls == ['_f']
    assert (moved['unit'], moved['name'], moved['address']) == (
        'unsorted/tFoo.cpp', 'tFoo::f', '0x00401000')
    assert moved['score'] == 75.0


def test_every_harness_module_is_part_of_the_judge():
    names = {os.path.basename(f) for f in cache.judge_files()}
    assert {'cache.py', 'claims.py', 'matched.py', 'paths.py', 'verify.py'} <= names


def test_a_changed_object_or_judge_is_compared_again(scoring, monkeypatch):
    score, calls = scoring
    score()
    score(code=b'\x90\xc3\xc3\xc3')
    assert len(calls) == 2
    monkeypatch.setattr(cache, '_memo', {'judge': 'another judge'})
    score()
    assert len(calls) == 3


def test_no_cache_compares_every_time(scoring, monkeypatch):
    score, calls = scoring
    monkeypatch.setenv('HARNESS_NO_CACHE', '1')
    score()
    score()
    assert len(calls) == 2


def test_a_score_is_keyed_to_the_exe_it_was_scored_against(cached, tmp_path):
    obj = tmp_path / 'u.obj'
    obj.write_bytes(bytes(coff.write_object(b'\xc3', '_f', [])))

    class Exe:
        def __init__(self, sha):
            self.sha256 = sha
    one = cache.score_key(str(obj), '_f', 0x401000, 1, Exe('a' * 64))
    assert one == cache.score_key(str(obj), '_f', 0x401000, 1, Exe('a' * 64))
    assert one != cache.score_key(str(obj), '_f', 0x401000, 1, Exe('b' * 64))
    assert cache.score_key(str(obj), '_f', 0x401000, 1, object()) is None


@pytest.mark.parametrize('text', ['{"result": {}}', '[]', '{"result": 1, "side_by_side": ""}',
                                  'not json'])
def test_a_malformed_entry_is_a_miss(cached, text):
    key = 'ab' + '0' * 62
    path = cache._score_path(key)
    os.makedirs(os.path.dirname(path))
    with open(path, 'w') as f:
        f.write(text)
    assert cache.load_score(key) is None


def test_an_entry_that_cannot_be_written_is_left_out(cached, monkeypatch):
    monkeypatch.setattr(cache, '_score_path', lambda key: '/nonexistent-dir/x/' + key + '.json')
    cache.store_score('ab' + '0' * 62, {'result': {}, 'side_by_side': ''})


def test_a_quoted_include_is_also_looked_for_beside_the_files_that_included_it(tmp_path):
    """MSVC: beside the including file, then beside each file above it, then the /I folders."""
    tree(tmp_path, {'u.cpp': '#include "sub/a.h"\n', 'sub/a.h': '#include "b.h"\n',
                    'b.h': '', 'inc/b.h': ''})
    missing = []
    deps = cache.dependencies(str(tmp_path / 'u.cpp'), [str(tmp_path / 'inc')], missing)
    assert os.path.realpath(tmp_path / 'b.h') in deps
    assert os.path.realpath(tmp_path / 'inc/b.h') not in deps
    assert os.path.normpath(os.path.realpath(tmp_path / 'sub') + '/b.h') in missing


def test_a_header_that_appears_earlier_on_the_search_rebuilds(unit, tmp_path):
    src, build, calls = unit
    (src / 'u.cpp').write_text('#include <u.h>\nint f() { return 1; }\n')
    first = build({'flags': '/O1 /Iearly /I.'})
    assert build({'flags': '/O1 /Iearly /I.'}).get('cached') and calls() == 1
    tree(src, {'early/u.h': 'int f(); // shadows ./u.h\n'})
    assert not build({'flags': '/O1 /Iearly /I.'}).get('cached') and calls() == 2
    assert first['ok']


def test_the_compilers_environment_is_part_of_the_key(unit, monkeypatch):
    src, build, calls = unit
    build()
    monkeypatch.setenv('CL', '/DX=1')
    assert not build().get('cached') and calls() == 2
