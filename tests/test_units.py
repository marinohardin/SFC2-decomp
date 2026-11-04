"""Tests for tools/units.py and the committed unit map (game/UNITS.tsv, game/, harness/units.json).

The fixture tests use invented functions; their expected rows are worked out by hand in the
comments. The committed-map tests read only committed files. One test regenerates the map from the
real evidence and is skipped where the SDK is absent.
"""
import bisect
import csv
import importlib.util
import json
import os
import pathlib
import struct

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _load():
    spec = importlib.util.spec_from_file_location('units', ROOT / 'tools/units.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


units = _load()

HEADER = ('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,headers,'
          'is_library,library,kind,tags\n')
SHIP = r'F:\Projects\Taldren\Projects\SFC2_5\Source\ship\TestShip.cpp'
SHIP_CASE = r'F:\Projects\Taldren\Projects\SFC2_5\Source\Ship\TestShip.cpp'
FIXTURE = HEADER + '\n'.join([
    # 0-3: two assert-path anchors with an unnamed function between them, then an EH funclet
    # directly after: Source/ship/TestShip.cpp, 4 functions ("ship" outvotes "Ship" 2 to 1).
    f'00401000,16,tTestShip::mFire,,,rtti-class-recovery,{SHIP},assert-path,,0,,function,',
    '00401100,16,FUN_00401100,,,default,,,,0,,function,',
    f'00401200,16,tTestShip::mStop,,,rtti-class-recovery,{SHIP},assert-path,,0,,function,',
    '00401300,16,Unwind@00401300,,,eh-analyzer,,,,0,,eh-funclet,',
    # 4-6: a class run; the relative assertion path in the middle is inlined code, not a unit:
    # unsorted/tTestHull.cpp, 3 functions.
    '00401400,16,tTestHull::mRepair,,,rtti-class-recovery,,,,0,,function,',
    r'00401500,16,FUN_00401500,,,default,..\Common\DataStore\DataStoreBuffer.cpp,assert-path,,0,,function,',
    '00401600,16,tTestHull::mPatch,,,rtti-class-recovery,,,,0,,function,',
    # 7: nothing known: unsorted/fn_00401000.cpp.
    '00401700,16,FUN_00401700,,,default,,,,0,,function,',
    # 8-10: two sdk-member anchors; the stray TestShip anchor between them is TestShip's smaller
    # run (1 against 2), so it is dropped and the function filled: Common/TestTimer.cpp, 3
    # functions, with its header from the headers column.
    r'00402010,16,FUN_00402010,,,sdklib-exact,,,..\..\Common\TestTimer.h,0,,function,',
    f'00402020,16,tTestShip::mStray,,,rtti-class-recovery,{SHIP_CASE},assert-path,,0,,function,',
    '00402030,16,FUN_00402030,,,sdklib-exact,,,,0,,function,',
    # 11: a library function: no class, so the page: unsorted/fn_00402000.cpp.
    '00402040,16,_teststrlen,,,ghidra-fid-vs,,,,1,crt,function,',
    # 12-14: a new page; tTestHull's second run (1 against 3) is dropped and a thunk has no
    # class: unsorted/fn_00403000.cpp, 3 functions.
    '00403000,16,FUN_00403000,,,default,,,,0,,function,',
    '00403100,16,tTestHull::mLate,,,rtti-class-recovery,,,,0,,function,',
    '00403200,5,tTestHull::~tTestHull,,,default,,,,0,,thunk,',
    # 15-17: a class run splits a page: fn_00404000.cpp, unsorted/tTestDock.cpp, then the
    # page's second run is named by its first address: fn_00404200.cpp.
    '00404000,16,FUN_00404000,,,default,,,,0,,function,',
    '00404100,16,tTestDock::mOpen,,,rtti-class-recovery,,,,0,,function,',
    '00404200,16,FUN_00404200,,,default,,,,0,,function,',
]) + '\n'
SDK_UNITS = {0x00402010: {'Common/TestTimer.cpp'}, 0x00402030: {'Common/TestTimer.cpp'},
             # two units for one address is no evidence
             0x00401700: {'Common/TestA.cpp', 'Tiger/TestB.cpp'}}

EXPECTED = [
    ('Source/ship/TestShip.cpp', '0x00401000', '0x00401300', 4, 'assert-path', ''),
    ('unsorted/tTestHull.cpp', '0x00401400', '0x00401600', 3, 'class', ''),
    ('unsorted/fn_00401000.cpp', '0x00401700', '0x00401700', 1, 'page', ''),
    ('Common/TestTimer.cpp', '0x00402010', '0x00402030', 3, 'sdk-member', 'Common/TestTimer.h'),
    ('unsorted/fn_00402000.cpp', '0x00402040', '0x00402040', 1, 'page', ''),
    ('unsorted/fn_00403000.cpp', '0x00403000', '0x00403200', 3, 'page', ''),
    ('unsorted/fn_00404000.cpp', '0x00404000', '0x00404000', 1, 'page', ''),
    ('unsorted/tTestDock.cpp', '0x00404100', '0x00404100', 1, 'class', ''),
    ('unsorted/fn_00404200.cpp', '0x00404200', '0x00404200', 1, 'page', ''),
]


def _fixture_fns(tmp_path):
    p = tmp_path / 'functions.csv'
    p.write_text(FIXTURE)
    return units.read_functions(str(p))


def _rows(tmp_path):
    return units.build(_fixture_fns(tmp_path), SDK_UNITS)


# --- the rules, on the fixture ----------------------------------------------------------------

def test_fixture_map_follows_the_evidence_rules(tmp_path):
    got = [(r['unit'], r['first'], r['last'], r['functions'], r['evidence'], r['header'])
           for r in _rows(tmp_path)]
    assert got == EXPECTED
    assert {r['profile'] for r in _rows(tmp_path)} == {'sdk-release'}


def test_original_paths_map_to_the_game_tree():
    assert units.game_path(r'F:\Projects\Taldren\Projects\SFC2_5\Source\app\shellApp.cpp') == 'Source/app/shellApp.cpp'
    assert units.game_path(r'F:\Projects\Taldren\Projects\SFC2_5\Meta\Servers\WonLogon.cpp') == 'Meta/Servers/WonLogon.cpp'
    assert units.game_path(r'F:\Projects\Taldren\Projects\Common\Thread\Thread.cpp') == 'Common/Thread/Thread.cpp'
    assert units.game_path(r'F:\Projects\Taldren\Projects\Switch\Switch.cpp') == 'Switch/Switch.cpp'
    assert units.game_path(r'D:\work\QSI\Projects\QSIInc\Tiger\tgSound.cpp') == 'Tiger/tgSound.cpp'
    assert units.game_path(r'D:\work\QSI\Projects\CORE\dmp3d\common\texBmp.cpp') == 'dmp3d/common/texBmp.cpp'
    # the SDK objects' own build paths
    assert units.game_path(r'C:\Projects\Taldren\Projects\SFC2_5\SCRIPTS\Shared\Source\Scripting\MapObject.cpp') == 'Source/Scripting/MapObject.cpp'
    assert units.game_path(r'C:\Projects\Taldren\Projects\SFC2_5\SCRIPTS\Shared\Source\Taldren\Common\Log\Log.cpp') == 'Common/Log/Log.cpp'
    assert units.game_path(r'C:\Gold\Qsi\Projects\Core\dmp3d\dx6\d3dutil.cpp') == 'dmp3d/dx6/d3dutil.cpp'
    # relative paths are inlined code; other roots are not the game's
    assert units.game_path(r'.\DataStore\DataStoreInput.cpp') is None
    assert units.game_path(r'..\Common\Thread\Optex.cpp') is None
    assert units.game_path(r'D:\Work\Sfc\crypto31\secshare.cpp') is None
    assert units.header_path(r'..\..\Common\DataBase\Database.h') == 'Common/DataBase/Database.h'


def test_spelling_prefers_the_exe_over_header_names():
    spell = units._spelling([('Common/Database/Database.cpp', 'assert-path'),
                             ('Common/DataBase/Database.h', 'header'),
                             ('Common/DataBase/DatabaseID.h', 'header')])
    assert spell('common/database/database.h') == 'Common/Database/Database.h'
    assert spell('common/database/databaseid.h') == 'Common/Database/DatabaseID.h'


def test_a_unit_kept_in_two_places_keeps_its_largest_run():
    # A A B A B B B: B's largest run (3) goes first and drops B at 2; A is then one run of 3.
    labels = {0: 'a', 1: 'a', 2: 'b', 3: 'a', 4: 'b', 5: 'b', 6: 'b'}
    assert units._resolve(labels) == {0: 'a', 1: 'a', 3: 'a', 4: 'b', 5: 'b', 6: 'b'}


def _coff(file_name, functions):
    """A minimal i386 COFF object: one .file symbol, then per function a section-definition
    symbol with its COMDAT selection and an external function symbol in that section."""
    syms, strtab = [], b''

    def name8(n):
        nonlocal strtab
        if len(n) <= 8:
            return n.encode().ljust(8, b'\0')
        off = 4 + len(strtab)
        strtab += n.encode() + b'\0'
        return b'\0\0\0\0' + struct.pack('<I', off)

    fname = file_name.encode()
    naux = (len(fname) + 17) // 18
    syms.append(struct.pack('<8sIhHBB', b'.file\0\0\0', 0, -2, 0, 103, naux) + fname.ljust(18 * naux, b'\0'))
    for i, (sym, selection) in enumerate(functions, 1):
        aux = struct.pack('<IHHIHB3s', 16, 0, 0, 0, 0, selection, b'')
        syms.append(struct.pack('<8sIhHBB', b'.text\0\0\0', 0, i, 0, 3, 1) + aux)
        syms.append(struct.pack('<8sIhHBB', name8(sym), 0, i, 0x20, 2, 0))
    table = b''.join(syms)
    nsym = len(table) // 18
    nsec = len(functions)
    head = struct.pack('<HHIIIHH', 0x14c, nsec, 0, 20 + 40 * nsec, nsym, 0, 0)
    sections = b''.join(struct.pack('<8sIIIIIIHHI', b'.text', 0, 0, 0, 0, 0, 0, 0, 0, 0)
                        for _ in range(nsec))
    return head + sections + table + struct.pack('<I', 4 + len(strtab)) + strtab


def _archive(members):
    out = b'!<arch>\n'
    for name, body in members:
        out += name.encode().ljust(16) + b' ' * 32 + str(len(body)).encode().ljust(10) + b'`\n' + body
        if len(body) & 1:
            out += b'\n'
    return out


def test_sdk_objects_give_the_unit_of_their_own_functions_only(tmp_path):
    obj = _coff(r'C:\Projects\Taldren\Projects\Common\TestTimer.cpp',
                [('?mTick@tTestTimer@@QAEXXZ', 1), ('??0tTestVector@@QAE@XZ', 2)])
    main, others, functions = units.object_evidence(obj)
    assert main == r'C:\Projects\Taldren\Projects\Common\TestTimer.cpp'
    assert others == []
    assert functions == {'?mTick@tTestTimer@@QAEXXZ': False, '??0tTestVector@@QAE@XZ': True}

    sdk = tmp_path / 'sdk'
    (sdk / 'Scripts' / 'Libraries').mkdir(parents=True)
    (sdk / 'Scripts' / 'Libraries' / 'TestR.lib').write_bytes(_archive([('TestTimer.obj/', obj)]))
    matches = tmp_path / 'matches.tsv'
    matches.write_text('00402010\t16\t?mTick@tTestTimer@@QAEXXZ\n'
                       '00402050\t16\t??0tTestVector@@QAE@XZ\n')
    by_address, _ = units.sdk_evidence(str(sdk), str(matches))
    # the select-any constructor gives no unit
    assert by_address == {0x00402010: {'Common/TestTimer.cpp'}, 0x00402050: set()}


# --- writing the tree ---------------------------------------------------------------------------

def test_placeholder_text():
    row = dict(unit='Common/TestTimer.cpp', first='0x00402010', last='0x00402030', functions=3,
               evidence='sdk-member', profile='sdk-release', header='Common/TestTimer.h')
    assert units.placeholder(row) == (
        "// Placeholder written by tools/units.py. Delete this block when the unit's first\n"
        "// function lands; the generator never touches a file with content.\n"
        "// Unit:      Common/TestTimer.cpp\n"
        "// Functions: 0x00402010 to 0x00402030, 3 in game/UNITS.tsv\n"
        "// Evidence:  sdk-member (an SDK library object compiled from this file has these functions)\n"
        "// Profile:   sdk-release (harness/units.json)\n")


def test_only_the_exact_generated_text_is_a_placeholder():
    old = ("// Placeholder written by tools/units.py. Delete this block when the unit's first\n"
           "// function lands; the generator never touches a file with content.\n"
           "// Unit:      unsorted/tTestDock.cpp\n"
           "// Functions: 0x00404100 to 0x00404180, 2 in game/UNITS.tsv\n"
           "// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)\n"
           "// Profile:   sdk-release (harness/units.json)\n")
    assert units.is_placeholder(old)                                 # an older range: still ours
    assert not units.is_placeholder(old + '// tTestDock is the dock screen; see 0x00404100.\n')
    assert not units.is_placeholder(old.replace('// Profile:', '// Profile: ').rstrip('\n'))
    assert not units.is_placeholder(units.MARKER + '\n// a note of an author\n')
    header = ("// Placeholder written by tools/units.py. Delete this block when the header's first\n"
              "// declaration lands; the generator never touches a file with content.\n"
              "// Header of: Common/TestTimer.cpp\n")
    assert units.is_placeholder(header)
    assert not units.is_placeholder(header + '\n')


def test_a_note_under_the_marker_is_content(tmp_path):
    rows = _rows(tmp_path)
    game = tmp_path / 'game'
    units.write_tree(rows, str(game))
    noted = game / 'unsorted' / 'tTestDock.cpp'
    noted.write_text(noted.read_text() + '// tTestDock is the dock screen.\n')
    later = [r for r in rows if r['unit'] != 'unsorted/tTestDock.cpp']
    assert units.write_tree(later, str(game)) == ([], [], [])        # not deleted
    assert units.write_tree(rows, str(game)) == ([], [], ['unsorted/tTestDock.cpp'])  # not rewritten
    assert noted.read_text().endswith('// tTestDock is the dock screen.\n')


def test_regeneration_is_idempotent_and_never_deletes_content(tmp_path):
    rows = _rows(tmp_path)
    game = tmp_path / 'game'
    written, removed, kept = units.write_tree(rows, str(game))
    assert len(written) == 10 and removed == [] and kept == []      # 9 units and one header
    snapshot = {p: p.read_text() for p in game.rglob('*') if p.is_file()}
    assert units.write_tree(rows, str(game)) == ([], [], [])
    assert {p: p.read_text() for p in game.rglob('*') if p.is_file()} == snapshot

    # a unit with code is left alone, even when it leaves the map
    code = game / 'unsorted' / 'tTestHull.cpp'
    code.write_text('// FUNCTION: STARFLEETOP 0x00401400\nvoid tTestHull::mRepair() {}\n')
    later = [r for r in rows if r['unit'] not in ('unsorted/tTestHull.cpp', 'unsorted/tTestDock.cpp')]
    written, removed, kept = units.write_tree(later, str(game))
    assert written == [] and kept == []
    assert removed == ['unsorted/tTestDock.cpp']                    # a placeholder is removed
    assert code.exists()
    # and when it is in the map again its file is not rewritten
    assert units.write_tree(rows, str(game)) == (['unsorted/tTestDock.cpp'], [], ['unsorted/tTestHull.cpp'])


def test_files_owned_by_other_packages_are_never_written(tmp_path):
    row = dict(unit='Common/DynamicAllocator.cpp', first='0x006fc490', last='0x006fcb40',
               functions=16, evidence='sdk-member', profile='sdk-release', header='')
    game = tmp_path / 'game'
    (game / 'port').mkdir(parents=True)
    (game / 'port' / 'types.h').write_text(units.MARKER + '\n')
    assert units.write_tree([row], str(game)) == ([], [], [])
    assert not (game / 'Common' / 'DynamicAllocator.cpp').exists()
    assert (game / 'port' / 'types.h').exists()


def test_units_json_keeps_existing_entries_and_profiles(tmp_path):
    p = tmp_path / 'units.json'
    p.write_text(json.dumps({'module': 'STARFLEETOP', 'default_profile': None,
                             'profiles': {'sdk-release': {'compiler': '6.4'}, 'other': {}},
                             'units': {'Tiger/tTest.cpp': {'profile': 'other'}}}))
    rows = [dict(unit='Tiger/tTest.cpp', profile='sdk-release'),
            dict(unit='Common/TestTimer.cpp', profile='sdk-release')]
    units.update_units_json(rows, str(p))
    assert p.read_text() == (
        '{\n "module": "STARFLEETOP",\n "default_profile": null,\n "profiles": {\n'
        '  "sdk-release": {\n   "compiler": "6.4"\n  },\n  "other": {}\n },\n "units": {\n'
        '  "Common/TestTimer.cpp": {"profile": "sdk-release"},\n'
        '  "Tiger/tTest.cpp": {"profile": "other"}\n }\n}\n')


# --- the committed map --------------------------------------------------------------------------

def _committed():
    rows = units.read_tsv(str(ROOT / 'game' / 'UNITS.tsv'))
    with open(ROOT / 'ghidra' / 'exports' / 'functions.csv', newline='') as f:
        addresses = sorted(int(r['address'], 16) for r in csv.DictReader(f))
    return rows, addresses


def test_every_function_maps_to_exactly_one_unit():
    rows, addresses = _committed()
    firsts = [int(r['first'], 16) for r in rows]
    lasts = [int(r['last'], 16) for r in rows]
    unmapped = [a for a in addresses
                if (i := bisect.bisect_right(firsts, a) - 1) < 0 or a > lasts[i]]
    assert unmapped == []
    counts = [0] * len(rows)
    for a in addresses:
        counts[bisect.bisect_right(firsts, a) - 1] += 1
    assert counts == [r['functions'] for r in rows]
    assert sum(counts) == len(addresses)


def test_no_two_units_overlap_and_no_unit_repeats():
    rows, _ = _committed()
    spans = [(int(r['first'], 16), int(r['last'], 16)) for r in rows]
    assert all(lo <= hi for lo, hi in spans)
    assert all(a[1] < b[0] for a, b in zip(spans, spans[1:]))
    names = [r['unit'].lower() for r in rows]
    assert len(names) == len(set(names))                              # case-insensitive filesystems
    assert {r['evidence'] for r in rows} <= {'assert-path', 'sdk-member', 'class', 'page'}
    assert all(r['unit'].startswith('unsorted/') == (r['evidence'] in ('class', 'page')) for r in rows)


def test_placeholders_match_the_tsv():
    rows, _ = _committed()
    game = ROOT / 'game'
    expected = {}
    for r in rows:
        expected[r['unit']] = units.placeholder(r)
        if r['header']:
            expected[r['header']] = units.header_placeholder(r)
    for rel, text in expected.items():
        if not units._ours(rel):
            continue
        body = (game / rel).read_text(encoding='latin1')
        assert body == text or not units.is_placeholder(body), rel
    stray = [p.relative_to(game).as_posix() for p in game.rglob('*')
             if p.suffix in ('.cpp', '.h') and units._ours(p.relative_to(game).as_posix())
             and p.relative_to(game).as_posix() not in expected]
    assert stray == []


def test_units_json_profiles_every_unit_with_the_game_include_order():
    rows, _ = _committed()
    cfg = json.loads((ROOT / 'harness' / 'units.json').read_text())
    assert cfg['profiles']['sdk-release']['include_first'] == ['.', 'port']
    missing = [r['unit'] for r in rows if cfg['units'].get(r['unit'], {}).get('profile') not in cfg['profiles']]
    assert missing == []


def _sdk():
    try:
        return units.sdk_dir()                              # the same resolution as main()
    except Exception:
        return ''


def test_sdk_variable_is_honoured_as_main_honours_it(tmp_path, monkeypatch):
    monkeypatch.setenv('SFCOP_SDK', str(tmp_path))
    assert units.sdk_dir() == str(tmp_path)
    assert units.sdk_dir('/explicit/sdk') == '/explicit/sdk'


@pytest.mark.skipif(not os.path.isdir(os.path.join(_sdk(), 'Scripts', 'Libraries')),
                    reason='needs the SDK from the install')
def test_committed_map_is_what_the_evidence_gives():
    assert units.main(['--check']) == 0
