"""One symbol, one exe address: exact functions that disagree are reported."""
from conftest import FakeIndex
from harness.lib import funcs, paths, verify
from harness.lib.verify import shared_symbol_conflicts


def res(address, unit, relocs, exact=True):
    return dict(address=address, unit=unit, exact=exact,
                relocs=[dict(symbol=s, exe_address=a, status=st) for s, a, st in relocs])


def test_one_method_name_at_two_addresses_is_reported():
    out = shared_symbol_conflicts([
        res('0x004b2000', 'unsorted/tMyHud3D.cpp',
            [('?mForceTranslate@tMatrix@@QAEXMMM@Z', '0x0069c920', 'unverified')]),
        res('0x00500000', 'unsorted/tOther.cpp',
            [('?mForceTranslate@tMatrix@@QAEXMMM@Z', '0x0069c870', 'unverified')]),
    ])
    assert out == ['?mForceTranslate@tMatrix@@QAEXMMM@Z is used at more than one exe address by '
                   'exact functions: 0x0069c870 by 0x00500000; 0x0069c920 by 0x004b2000 '
                   '(one of them names the wrong symbol)']


def test_agreeing_uses_are_not_reported():
    out = shared_symbol_conflicts([
        res('0x00401000', 'a.cpp', [('?g@@3HA', '0x00500010', 'ok')]),
        res('0x00402000', 'b.cpp', [('?g@@3HA', '0x00500010', 'unverified')]),
    ])
    assert out == []


def test_literals_and_compiler_statics_are_compared_within_a_unit_only():
    out = shared_symbol_conflicts([
        res('0x00401000', 'a.cpp', [('__real@4@3ffccccccd0000000000', '0x00796418', 'unverified'),
                                    ('_$E2', '0x0040ceee', 'unverified')]),
        res('0x00402000', 'b.cpp', [('__real@4@3ffccccccd0000000000', '0x0079c988', 'unverified'),
                                    ('_$E2', '0x00438606', 'unverified')]),
    ])
    assert out == []


def test_a_literal_at_two_addresses_in_one_unit_is_reported():
    out = shared_symbol_conflicts([
        res('0x00401000', 'a.cpp', [('__real@4@3ffccccccd0000000000', '0x00796418', 'unverified')]),
        res('0x00401100', 'a.cpp', [('__real@4@3ffccccccd0000000000', '0x0079c988', 'unverified')]),
    ])
    assert len(out) == 1 and out[0].startswith('__real@4@3ffccccccd0000000000 is used at')


def test_partial_functions_and_mismatched_relocations_are_left_out():
    out = shared_symbol_conflicts([
        res('0x00401000', 'a.cpp', [('?h@@YAXXZ', '0x00401200', 'ok')]),
        res('0x00402000', 'a.cpp', [('?h@@YAXXZ', '0x00401300', 'unverified')], exact=False),
        res('0x00403000', 'a.cpp', [('?h@@YAXXZ', '0x00401400', 'mismatch')]),
    ])
    assert out == []


def test_per_unit_and_global_symbols_mixed_in_one_run():
    out = shared_symbol_conflicts([
        res('0x00401000', 'a.cpp', [('_$E2', '0x0040ceee', 'unverified'),
                                    ('?g@@3HA', '0x00500010', 'ok')]),
        res('0x00402000', 'b.cpp', [('?g@@3HA', '0x00500014', 'unverified')]),
    ])
    assert len(out) == 1 and out[0].startswith('?g@@3HA is used at')


def test_a_static_symbol_is_compared_within_its_unit_only():
    # Two units may each define a static function under one mangled name.
    statics = [dict(symbol='?helper@@YAXXZ', exe_address=a, status='unverified', file_local=True)
               for a in ('0x00401500', '0x00402500')]
    out = shared_symbol_conflicts([
        dict(address='0x00401000', unit='a.cpp', exact=True, relocs=[statics[0]]),
        dict(address='0x00402000', unit='b.cpp', exact=True, relocs=[statics[1]]),
    ])
    assert out == []


def test_a_conflict_fails_the_run_with_an_error_line():
    rep = verify.Report()
    rep.results = [res('0x00401000', 'a.cpp', [('?g@@3HA', '0x00500010', 'ok')]),
                   res('0x00402000', 'b.cpp', [('?g@@3HA', '0x00500014', 'ok')])]
    verify.report_shared_symbols(rep)
    assert rep.failed and rep.lines[0].startswith('ERROR ?g@@3HA is used at more than one')


def test_run_reports_shared_symbols_and_main_exits_2(tmp_path, monkeypatch):
    monkeypatch.setattr(paths, 'SRC_ROOT', str(tmp_path))
    monkeypatch.setattr(paths, 'MATCHED_SET', str(tmp_path / 'matched_set.json'))
    monkeypatch.setattr(paths, 'BUILD', str(tmp_path / 'build'))
    monkeypatch.setattr(funcs, 'FunctionIndex', lambda *a: FakeIndex())
    monkeypatch.setattr(verify, 'check_exe', lambda rep: None)   # no retail file is read
    calls = []

    def conflict(rep):
        calls.append(rep)
        rep.add('ERROR', 'x is used at more than one exe address')
        rep.failed = True
    monkeypatch.setattr(verify, 'report_shared_symbols', conflict)
    assert verify.main(['--all']) == verify.EXIT_FAILED
    assert len(calls) == 1
