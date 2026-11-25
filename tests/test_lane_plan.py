"""Tests for tools/lane_plan.py, on an invented source tree built under tmp_path."""
import importlib.util
import io
import json
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('lane_plan', ROOT / 'tools/lane_plan.py')
lane_plan = importlib.util.module_from_spec(spec)
spec.loader.exec_module(lane_plan)


def build(tmp):
    """Units A and B own aa.h; B and C share base.h; D has its own header dd.h."""
    files = {
        'op/u/A.cpp': '#include "u/aa.h"\n',
        'op/u/B.cpp': '#include "u/aa.h"\n#include "u/base.h"\n',
        'op/u/C.cpp': '#include "u/base.h"\n',
        'op/u/D.cpp': '#include "u/dd.h"\n#include <windows.h>\n',
        'op/u/aa.h': '#include "base.h"\nclass tOwned : public tBase {\n};\n',
        'op/u/base.h': '// class tCommented { };\nstruct tBase {\n};\n',
        'op/u/dd.h': 'class tDee {\n};\n',
    }
    for p, text in files.items():
        f = tmp / p
        f.parent.mkdir(parents=True, exist_ok=True)
        f.write_text(text)
    (tmp / 'op/UNITS.tsv').write_text(
        'unit\tfirst\tlast\tfunctions\tevidence\tprofile\theader\n'
        'u/A.cpp\t0x00401000\t0x004010ff\t3\tclass\tx\t\n'
        'u/B.cpp\t0x00401100\t0x004011ff\t1\tclass\tx\t\n'
        'u/C.cpp\t0x00401200\t0x004012ff\t1\tclass\tx\t\n'
        'u/D.cpp\t0x00401300\t0x004013ff\t1\tclass\tx\t\n')
    (tmp / 'op/manifest.tsv').write_text('address\tsize\tsymbol\tsource\tmodes\tdecorated\n'
                                         '0x00401000\t10\ta\tu/A.cpp\tvc6\tx\n')
    sym = ['address\tkind\tname\tclass\tsymbol\tsymbol_evidence\tdeclaration\tevidence']
    for a, c in [('0x00401000', 'tOwned'), ('0x00401010', 'tOwned'), ('0x00401020', 'tBase'),
                 ('0x00401100', 'tGhost'), ('0x00401300', 'tDee')]:
        sym.append('%s\tfunction\tfn\t%s\t\t\t\t' % (a, c))
    sym.append('0x00401030\tthunk\tfn\ttBase\t\t\t\t')
    (tmp / 'analysis/symbols').mkdir(parents=True)
    (tmp / 'analysis/symbols/STARFLEETOP.tsv').write_text('\n'.join(sym) + '\n')
    (tmp / 'analysis/results').mkdir(parents=True)
    (tmp / 'ghidra/exports').mkdir(parents=True)
    (tmp / 'ghidra/exports/functions.csv').write_text('address,size,name\n00401010,16,x\n')
    (tmp / 'analysis/results/matched_set.json').write_text(json.dumps(
        {'version': 1, 'functions': {'0x00401000': {'exact': True, 'size': 10}}}))


def run(tmp, argv):
    out = io.StringIO()
    code = lane_plan.main(argv + ['--root', str(tmp)], out=out)
    return code, out.getvalue()


def test_classifies_headers_and_blocks_functions(tmp_path):
    build(tmp_path)
    (tmp_path / 'one.txt').write_text('u/A.cpp\nop/u/B.cpp\n# comment\n')
    code, text = run(tmp_path, ['--units', str(tmp_path / 'one.txt'), '--max-blocked', '0.9'])
    # Non-exact: tOwned 0x...1010 (aa.h, owned), tBase 0x...1020 (base.h, also in C),
    # tGhost 0x...1100 (declared nowhere). The thunk is ignored; 0x...1000 is exact.
    assert 'lane: 2 units, 3 non-exact functions: 1 workable, 2 blocked (67%)' in text
    assert '1 owned, 1 shared, 0 contested' in text
    assert 'blocked   1  u/base.h  (1 units outside the lane include it)' in text
    assert 'blocked   1  (undeclared)' in text
    assert code == 0


def test_threshold_exit_code(tmp_path):
    build(tmp_path)
    (tmp_path / 'one.txt').write_text('u/A.cpp\nu/B.cpp\n')
    code, _ = run(tmp_path, ['--units', str(tmp_path / 'one.txt')])
    assert code == 1


def test_two_lanes_contest_a_header_and_write_files(tmp_path):
    build(tmp_path)
    (tmp_path / 'x.txt').write_text('u/A.cpp\n')
    (tmp_path / 'y.txt').write_text('u/C.cpp\nu/D.cpp\n')
    out_dir = tmp_path / 'out'
    code, text = run(tmp_path, ['--lane', 'x=' + str(tmp_path / 'x.txt'),
                                '--lane', 'y=' + str(tmp_path / 'y.txt'),
                                '--max-blocked', '1', '--write-files', str(out_dir)])
    # aa.h is also included by B (outside both lanes); base.h is reached from A (through aa.h)
    # and from C: contested for both lanes. D owns dd.h.
    assert 'headers: 0 owned, 1 shared, 1 contested' in text
    assert 'headers: 1 owned, 0 shared, 1 contested' in text
    assert code == 0
    assert (out_dir / 'files-x.txt').read_text() == 'op/u/A.cpp\n'
    lines = (out_dir / 'functions-x.tsv').read_text().splitlines()
    assert lines[0] == 'address\tsize\tclass\tunit\tstatus\tblocking_header'
    assert '0x00401010\t16\ttOwned\tu/A.cpp\tblocked\tu/aa.h' in lines
    assert '0x00401020\t\ttBase\tu/A.cpp\tblocked\tu/base.h' in lines
    assert '0x00401300\t\ttDee\tu/D.cpp\tworkable\t' in (
        out_dir / 'functions-y.tsv').read_text().splitlines()


def test_unit_in_two_lanes_is_a_conflict(tmp_path):
    build(tmp_path)
    (tmp_path / 'x.txt').write_text('u/A.cpp\n')
    (tmp_path / 'y.txt').write_text('u/A.cpp\n')
    code, text = run(tmp_path, ['--lane', 'x=' + str(tmp_path / 'x.txt'),
                                '--lane', 'y=' + str(tmp_path / 'y.txt'), '--max-blocked', '1'])
    assert code == 1
    assert 'CONFLICT: unit u/A.cpp is in lanes x, y' in text


def test_unknown_unit_is_a_usage_error(tmp_path):
    build(tmp_path)
    (tmp_path / 'x.txt').write_text('u/Nope.cpp\n')
    code, _ = run(tmp_path, ['--units', str(tmp_path / 'x.txt')])
    assert code == 2


def extend(tmp):
    """Unit E: class tPort declared under op/port, class tLoose in a header no unit includes, a
    function with no class; unit E2: a header found beside its includer (u/sub/inner.h)."""
    for p, text in {
        'op/port/pp.h': 'class tPort {\n};\n',
        'op/u/loose.h': 'class tLoose {\n};\n',
        'op/u/E.cpp': '#include "pp.h"\n',
        'op/u/sub/E2.cpp': '#include "inner.h"\n',
        'op/u/sub/inner.h': 'class tInner {\n};\n',
    }.items():
        f = tmp / p
        f.parent.mkdir(parents=True, exist_ok=True)
        f.write_text(text)
    with open(tmp / 'op/UNITS.tsv', 'a') as f:
        f.write('u/E.cpp\t0x00401400\t0x004014ff\t4\tclass\tx\t\n'
                'u/sub/E2.cpp\t0x00401500\t0x004015ff\t1\tclass\tx\t\n')
    with open(tmp / 'analysis/symbols/STARFLEETOP.tsv', 'a') as f:
        for a, c in [('0x00401400', 'tPort'), ('0x00401410', 'tLoose'), ('0x00401420', ''),
                     ('0x00401500', 'tInner')]:
            f.write('%s\tfunction\tfn\t%s\t\t\t\t\n' % (a, c))
    with open(tmp / 'ghidra/exports/functions.csv', 'a') as f:
        f.write('00401420,8,y\n')


def rows(tmp, units, extra=()):
    (tmp / 'e.txt').write_text(units)
    out_dir = tmp / 'o'
    code, text = run(tmp, ['--lane', 'e=' + str(tmp / 'e.txt'), '--max-blocked', '1',
                           '--write-files', str(out_dir)] + list(extra))
    return code, text, (out_dir / 'functions-e.tsv').read_text().splitlines()


def test_port_header_is_a_hotspot_and_blocks(tmp_path):
    build(tmp_path)
    extend(tmp_path)
    code, text, lines = rows(tmp_path, 'u/E.cpp\n')
    assert '0 owned, 0 shared, 0 contested, 1 hotspot' in text
    assert '0x00401400\t\ttPort\tu/E.cpp\tblocked\tport/pp.h' in lines
    assert 'op/port/pp.h' not in (tmp_path / 'o/files-e.txt').read_text()


def test_class_in_a_header_no_unit_includes_and_empty_class(tmp_path):
    build(tmp_path)
    extend(tmp_path)
    _, _, lines = rows(tmp_path, 'u/E.cpp\n')
    # u/loose.h is included by no unit, so it is the lane's to edit and goes on its file list.
    assert '0x00401410\t\ttLoose\tu/E.cpp\tworkable\t' in lines
    assert 'op/u/loose.h' in (tmp_path / 'o/files-e.txt').read_text()
    assert '0x00401420\t8\t\tu/E.cpp\tworkable\t' in lines


def test_include_resolved_beside_the_includer(tmp_path):
    build(tmp_path)
    extend(tmp_path)
    _, text, lines = rows(tmp_path, 'u/sub/E2.cpp\n')
    assert '1 owned, 0 shared, 0 contested' in text
    assert '0x00401500\t\ttInner\tu/sub/E2.cpp\tworkable\t' in lines


def test_grant_gives_a_shared_header_to_one_lane(tmp_path):
    build(tmp_path)
    (tmp_path / 'x.txt').write_text('u/A.cpp\n')
    (tmp_path / 'y.txt').write_text('u/C.cpp\n')
    out_dir = tmp_path / 'o'
    code, text = run(tmp_path, ['--lane', 'x=' + str(tmp_path / 'x.txt'),
                                '--lane', 'y=' + str(tmp_path / 'y.txt'), '--max-blocked', '1',
                                '--grant', 'x=u/aa.h', '--grant', 'y=op/u/base.h',
                                '--write-files', str(out_dir)])
    assert code == 0
    assert 'granted u/aa.h: 1 units outside the lane include it' in text
    assert 'op/u/aa.h' in (out_dir / 'files-x.txt').read_text()
    assert 'op/u/base.h' in (out_dir / 'files-y.txt').read_text()
    assert 'op/u/base.h' not in (out_dir / 'files-x.txt').read_text()
    lines = (out_dir / 'functions-x.tsv').read_text().splitlines()
    assert '0x00401010\t16\ttOwned\tu/A.cpp\tworkable\t' in lines
    assert '0x00401020\t\ttBase\tu/A.cpp\tblocked\tu/base.h' in lines


def test_grant_of_a_port_header_is_refused(tmp_path):
    build(tmp_path)
    extend(tmp_path)
    (tmp_path / 'x.txt').write_text('u/E.cpp\n')
    code, _ = run(tmp_path, ['--lane', 'x=' + str(tmp_path / 'x.txt'), '--grant', 'x=port/pp.h'])
    assert code == 2


def test_a_header_as_a_unit_and_a_bad_lane_name_exit_2(tmp_path):
    build(tmp_path)
    (tmp_path / 'x.txt').write_text('u/aa.h\n')
    assert run(tmp_path, ['--units', str(tmp_path / 'x.txt')])[0] == 2
    (tmp_path / 'y.txt').write_text('u/A.cpp\n')
    code, _ = run(tmp_path, ['--lane', 'a/b=' + str(tmp_path / 'y.txt'),
                             '--write-files', str(tmp_path / 'o')])
    assert code == 2


def test_two_lanes_blocked_by_one_header_is_not_a_conflict(tmp_path):
    build(tmp_path)
    (tmp_path / 'x.txt').write_text('u/B.cpp\n')
    (tmp_path / 'y.txt').write_text('u/C.cpp\n')
    code, text = run(tmp_path, ['--lane', 'x=' + str(tmp_path / 'x.txt'),
                                '--lane', 'y=' + str(tmp_path / 'y.txt'), '--max-blocked', '1'])
    assert code == 0
    assert 'CONFLICT' not in text
