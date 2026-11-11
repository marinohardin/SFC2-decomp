"""Tests for tools/manifest.py: the manifest and the override table from the annotations in game/.

The fixture tree is invented. Its vtable slots and expected rows are worked out by hand in the
comments. One test checks the committed outputs against the committed tree.
"""
import importlib.util
import pathlib

import pytest

ROOT = pathlib.Path(__file__).resolve().parents[1]


def _load():
    spec = importlib.util.spec_from_file_location('manifest', ROOT / 'tools/manifest.py')
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


manifest = _load()

CSV = ('address,size,name,mangled,demangled,name_source,translation_unit,tu_evidence,headers,'
       'is_library,library,kind,tags\n'
       '00401000,24,tTestShip::mSpeed,,,default,,,,0,,function,\n'
       '00401100,16,tTestShip::Instance,,,default,,,,0,,function,\n'
       '00401200,40,tTestShip::mFire,,,default,,,,0,,function,\n'
       '00401300,52,tTestShip::mHull,,,default,,,,0,,function,\n'
       '00401400,8,TestRound,,,default,,,,0,,function,\n')

# Slots, by VC6's rule for single inheritance:
#   tTestBase  0 ~tTestBase, 1 mSpeed, 2 mStop
#   tTestShip  0 ~tTestShip (overrides 0), 1 mSpeed (overrides 1), 2 mStop (inherited), 3 mFire
HEADER = '''\
#include "port/types.h"

// VTABLE: STARFLEETOP 0x00790000
class tTestBase
{
	SFCOP_VFPTR // PORT: pointer size
public:
	virtual ~tTestBase();
	virtual int32 mSpeed() const;
	virtual void mStop() SFCOP_PURE; // PORT: pointer size
};

// VTABLE: STARFLEETOP 0x00790020
class tTestShip : public tTestBase
{
public:
	static tTestShip* Instance();
	virtual ~tTestShip();
	virtual int32 mSpeed() const;
	virtual void mFire( int32 count, const char* why );
	int32 mHull( const tTestShip& other ) const;

private:
	class tTestPart
	{
	public:
		int32 fMass;
	};
	ptr32( tTestShip ) fNext;
	int32 fSpeed;
};
'''

SOURCE = '''\
#include "Test/TestShip.h"

// FUNCTION: STARFLEETOP 0x00401000
int32 tTestShip::mSpeed() const
{
	return fSpeed;
}

// FUNCTION: STARFLEETOP 0x00401100
tTestShip* tTestShip::Instance()
{
	return 0;
}

// STUB: STARFLEETOP 0x00401200
void tTestShip::mFire( int32 count, const char* why )
{
}

// FUNCTION: STARFLEETOP 0x00401300
int32 tTestShip::mHull( const tTestShip& other ) const
{
	return other.fSpeed;
}

// FUNCTION: STARFLEETOP 0x00401400
int32 TestRound( int32 value )
{
	return value;
}
'''


# Every FUNCTION address the fixtures claim is recorded exact unless a test says otherwise.
EXACT = ('0x00401000', '0x00401100', '0x00401300', '0x00401400', '0x00401500', '0x00401600')


def make_tree(tmp_path, header=HEADER, source=SOURCE):
    game = tmp_path / 'game'
    (game / 'Test').mkdir(parents=True)
    (game / 'port').mkdir()
    (game / 'Test' / 'TestShip.h').write_text(header)
    (game / 'Test' / 'TestShip.cpp').write_text(source)
    # port/ is not scanned: an annotation there is ignored.
    (game / 'port' / 'types.h').write_text('// FUNCTION: STARFLEETOP 0x00409999\nvoid nope();\n')
    csv = tmp_path / 'functions.csv'
    csv.write_text(CSV)
    write_matched(tmp_path, EXACT)
    return game, csv



def write_matched(tmp_path, exact, partial=()):
    entries = ['"%s": {"exact": true, "score": 100.0}' % a for a in exact]
    entries += ['"%s": {"exact": false, "score": %s}' % (a, s) for a, s in partial]
    (tmp_path / 'matched.json').write_text('{"version": 1, "functions": {%s}}' % ', '.join(entries))


def run(tmp_path, game, csv, *extra):
    return manifest.main(['--game', str(game), '--functions', str(csv),
                          '--matched', str(tmp_path / 'matched.json'),
                          '--manifest', str(tmp_path / 'out' / 'manifest.tsv'),
                          '--override', str(tmp_path / 'out' / 'override.cpp'), *extra])


def test_manifest_lists_every_claim_in_address_order_with_size_and_modes(tmp_path):
    game, csv = make_tree(tmp_path)
    assert run(tmp_path, game, csv) == 0
    assert (tmp_path / 'out' / 'manifest.tsv').read_text() == (
        'address\tsize\tsymbol\tsource\tmodes\n'
        '0x00401000\t24\ttTestShip::mSpeed() const\tTest/TestShip.cpp\tvc6 guest\n'
        '0x00401100\t16\ttTestShip::Instance()\tTest/TestShip.cpp\tvc6 guest\n'
        '0x00401200\t40\ttTestShip::mFire(int32,const char*)\tTest/TestShip.cpp\tstub\n'
        '0x00401300\t52\ttTestShip::mHull(const tTestShip&) const\tTest/TestShip.cpp\tvc6 guest\n'
        '0x00401400\t8\tTestRound(int32)\tTest/TestShip.cpp\tvc6 guest\n')


def test_override_has_an_entry_point_per_guest_function_with_its_calling_convention(tmp_path):
    game, csv = make_tree(tmp_path)
    assert run(tmp_path, game, csv) == 0
    text = (tmp_path / 'out' / 'override.cpp').read_text()
    # __thiscall members through member<>(), __cdecl static and free functions through a cast.
    assert ('extern "C" void sub_00401000(void) {\n'
            '    ::sfcop::guest::entered(0);\n'
            '    ::sfcop::guest::Entry< ::sfcop::guest::member< int32() const >'
            '( &tTestShip::mSpeed ) >::run();\n}') in text
    assert ('    ::sfcop::guest::entered(1);\n'
            '    ::sfcop::guest::Entry< static_cast< tTestShip* (*)() >'
            '( &tTestShip::Instance ) >::run();') in text
    assert ('    ::sfcop::guest::entered(2);\n'
            '    ::sfcop::guest::Entry< ::sfcop::guest::member< int32( const tTestShip& ) const >'
            '( &tTestShip::mHull ) >::run();') in text
    assert ('    ::sfcop::guest::Entry< static_cast< int32 (*)( int32 ) >'
            '( &TestRound ) >::run();') in text
    # A STUB keeps its lifted body.
    assert 'sub_00401200' not in text
    assert '#include "Test/TestShip.h"' in text
    assert 'const unsigned sfcop_guest_override_count = 4u;' in text
    assert '    {0x00401300u, sub_00401300, "tTestShip::mHull(const tTestShip&) const", ' \
           '"Test/TestShip.cpp"},' in text


def test_virtual_members_without_a_body_get_a_dispatcher_at_their_slot(tmp_path):
    game, csv = make_tree(tmp_path)
    assert run(tmp_path, game, csv) == 0
    text = (tmp_path / 'out' / 'override.cpp').read_text()
    assert ('// slot 1 of tTestBase\'s vtable (0x00790000)\n'
            'int32 tTestBase::mSpeed() const {\n'
            '    return ::sfcop::guest::virtualCall< int32 >( this, 1 );\n}') in text
    assert ('// slot 2 of tTestBase\'s vtable (0x00790000)\n'
            'void tTestBase::mStop() {\n'
            '    return ::sfcop::guest::virtualCall< void >( this, 2 );\n}') in text
    # tTestShip::mSpeed has a body (FUNCTION), tTestShip::mFire a STUB body: no dispatcher; the
    # destructors never get one.
    assert 'tTestShip::mSpeed() const {' not in text
    assert 'tTestShip::mFire(' not in text
    assert '::~' not in text


def test_slots_follow_vc6_single_inheritance(tmp_path):
    game, csv = make_tree(tmp_path)
    _, classes = manifest.build(str(game), str(csv), str(tmp_path / 'matched.json'))
    slots = [(owner, m['name']) for owner, m in manifest.vtable_slots('tTestShip', classes)]
    assert slots == [('tTestShip', '~tTestShip'), ('tTestShip', 'mSpeed'),
                     ('tTestBase', 'mStop'), ('tTestShip', 'mFire')]
    assert 'tTestShip::tTestPart' in classes


def test_check_passes_when_current_and_fails_when_an_annotation_changes(tmp_path, capsys):
    game, csv = make_tree(tmp_path)
    assert run(tmp_path, game, csv) == 0
    assert run(tmp_path, game, csv, '--check') == 0
    (game / 'Test' / 'TestShip.cpp').write_text(SOURCE.replace('// FUNCTION: STARFLEETOP 0x00401400',
                                                               '// STUB: STARFLEETOP 0x00401400'))
    assert run(tmp_path, game, csv, '--check') == 1
    assert 'is not what the annotations give' in capsys.readouterr().err


@pytest.mark.parametrize('header,source,message', [
    (HEADER, SOURCE.replace('0x00401100', '0x00401000'), '0x00401000 claimed twice'),
    (HEADER, SOURCE.replace('0x00401400', '0x00402000'), '0x00402000 is not a function'),
    (HEADER.replace('virtual void mFire( int32 count, const char* why );',
                    'virtual void mFire( int32 count );\n\tvirtual void mFire( const char* why );'),
     SOURCE.replace('void tTestShip::mFire( int32 count, const char* why )',
                    'void tTestShip::mFire( int32 count )'),
     'overloaded virtual mFire'),
    (HEADER.replace('// VTABLE: STARFLEETOP 0x00790020\n', '// VTABLE: STARFLEETOP 0x00790020\n#define X\n'),
     SOURCE, 'must come right before its class'),
    (HEADER.replace('class tTestShip : public tTestBase', 'class tTestShip : public tTestBase, public tOther'),
     SOURCE, 'multiple bases'),
])
def test_what_the_scanner_cannot_read_is_refused(tmp_path, capsys, header, source, message):
    game, csv = make_tree(tmp_path, header, source)
    assert run(tmp_path, game, csv) == 2
    assert message in capsys.readouterr().err


def test_signatures_are_read_from_definition_heads():
    assert manifest.parse_signature('int32 X::Y::operator==( const char* raw_name ) const') == (
        'int32', 'X::Y::operator==', ('const char*',), ('raw_name',), True, None)
    assert manifest.parse_signature('virtual int32 operator<( const tFoo& ) const') == (
        'int32', 'operator<', ('const tFoo&',), ('a0',), True, None)
    assert manifest.parse_signature('void tFoo::mSet( unsigned int, int32 count = 3 )') == (
        'void', 'tFoo::mSet', ('unsigned int', 'int32'), ('a0', 'count'), False, None)
    with pytest.raises(manifest.NotAFunction):
        manifest.parse_signature('ptr32( tFoo ) fNext')
    assert manifest.parse_signature('long __stdcall tWin::mWndProcThunk( void* w, unsigned int m )') == (
        'long', 'tWin::mWndProcThunk', ('void*', 'unsigned int'), ('w', 'm'), False, '__stdcall')


def test_a_constructor_initialiser_list_is_not_part_of_the_signature(tmp_path):
    source = SOURCE.replace('// FUNCTION: STARFLEETOP 0x00401100\ntTestShip* tTestShip::Instance()',
                            '// FUNCTION: STARFLEETOP 0x00401100\ntTestShip* tTestShip::Instance() :\n'
                            '\tfSpeed( 0 )')
    game, csv = make_tree(tmp_path, source=source)
    functions, _ = manifest.build(str(game), str(csv), str(tmp_path / 'matched.json'))
    assert [f['name'] for f in functions if f['address'] == 0x00401100] == ['tTestShip::Instance']


def test_constructors_and_destructors_have_no_guest_entry_point(tmp_path):
    # C++ has no pointer to a constructor or destructor: the manifest keeps the function (vc6 only)
    # and the override table emits no entry for it.
    header = HEADER.replace('\tstatic tTestShip* Instance();', '\ttTestShip();\n\tstatic tTestShip* Instance();')
    source = SOURCE + ('\n// FUNCTION: STARFLEETOP 0x00401500\ntTestShip::tTestShip()\n{\n}\n'
                       '\n// FUNCTION: STARFLEETOP 0x00401600\ntTestShip::~tTestShip()\n{\n}\n')
    game, csv = make_tree(tmp_path, header=header, source=source)
    csv.write_text(CSV + '00401500,20,tTestShip::tTestShip,,,default,,,,0,,function,\n'
                         '00401600,12,tTestShip::~tTestShip,,,default,,,,0,,function,\n')
    assert run(tmp_path, game, csv) == 0
    rows = {l.split('\t')[0]: l.split('\t') for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}
    assert rows['0x00401500'][2:] == ['tTestShip::tTestShip()', 'Test/TestShip.cpp', 'vc6']
    assert rows['0x00401600'][2:] == ['tTestShip::~tTestShip()', 'Test/TestShip.cpp', 'vc6']
    assert rows['0x00401000'][4] == 'vc6 guest'
    text = (tmp_path / 'out' / 'override.cpp').read_text()
    assert 'sub_00401500' not in text and 'sub_00401600' not in text
    assert '&tTestShip::tTestShip' not in text and '&tTestShip::~tTestShip' not in text


def test_an_annotated_part_binding_claims_its_search_member(tmp_path):
    # GenAbstractBinding(X) in X's body defines the static members Search_X and Test_X; an
    # annotation on it claims Search_X, a static member with no parameters returning X*.
    header = HEADER.replace('\tstatic tTestShip* Instance();',
                            '\t// FUNCTION: STARFLEETOP 0x00401500\n'
                            '\tGenAbstractBinding( tTestShip );\n'
                            '\tstatic tTestShip* Instance();')
    game, csv = make_tree(tmp_path, header=header)
    csv.write_text(CSV + '00401500,117,tTestShip::Search_tTestShip,,,default,,,,0,,function,\n')
    assert run(tmp_path, game, csv) == 0
    rows = {l.split('\t')[0]: l.split('\t') for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}
    assert rows['0x00401500'][1:] == ['117', 'tTestShip::Search_tTestShip()', 'Test/TestShip.h', 'vc6 guest']
    text = (tmp_path / 'out' / 'override.cpp').read_text()
    assert 'static_cast< tTestShip * (*)() >( &tTestShip::Search_tTestShip )' in text
    _, classes = manifest.scan(str(game))
    assert [m['name'] for m in classes['tTestShip']['members'] if m['static']] == \
        ['Search_tTestShip', 'Test_tTestShip', 'Instance']


def test_a_part_binding_for_another_class_is_refused(tmp_path, capsys):
    header = HEADER.replace('\tstatic tTestShip* Instance();',
                            '\tGenAbstractBinding( tTestBase );\n\tstatic tTestShip* Instance();')
    game, csv = make_tree(tmp_path, header=header)
    assert run(tmp_path, game, csv) != 0
    assert 'GenAbstractBinding(tTestBase) inside class tTestShip' in capsys.readouterr().err


def test_a_function_not_recorded_exact_is_partial_and_has_no_entry_point(tmp_path):
    # 0x00401300 scored 99.5 and 0x00401400 was never recorded: both stay in game/ as partial, so
    # neither the object lift (vc6) nor the override table (guest) takes them. The exact functions
    # of the same unit still enter: /O1 gives each function its own section.
    game, csv = make_tree(tmp_path)
    write_matched(tmp_path, ('0x00401000', '0x00401100'), partial=[('0x00401300', 99.5)])
    assert run(tmp_path, game, csv) == 0
    rows = {l.split('\t')[0]: l.split('\t')[4] for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}
    assert rows == {'0x00401000': 'vc6 guest', '0x00401100': 'vc6 guest', '0x00401200': 'stub',
                    '0x00401300': 'partial', '0x00401400': 'partial'}
    text = (tmp_path / 'out' / 'override.cpp').read_text()
    assert 'sub_00401000' in text and 'sub_00401100' in text
    assert 'sub_00401300' not in text and 'sub_00401400' not in text
    assert 'sfcop_guest_override_count = 2u;' in text


def test_another_calling_convention_is_lifted_only(tmp_path):
    # A __stdcall definition (a window procedure, say) cannot be a guest entry point, which uses
    # the default conventions: exact it is vc6 only, not exact partial.
    source = SOURCE.replace('int32 TestRound( int32 value )', 'int32 __stdcall TestRound( int32 value )')
    game, csv = make_tree(tmp_path, source=source)
    write_matched(tmp_path, EXACT)
    assert run(tmp_path, game, csv) == 0
    rows = {l.split('\t')[0]: l.split('\t')[2:5] for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}
    assert rows['0x00401400'] == ['TestRound(int32)', 'Test/TestShip.cpp', 'vc6']
    assert 'sub_00401400' not in (tmp_path / 'out' / 'override.cpp').read_text()
    write_matched(tmp_path, ('0x00401000',))
    assert run(tmp_path, game, csv) == 0
    rows = {l.split('\t')[0]: l.split('\t')[4] for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}
    assert rows['0x00401400'] == 'partial'


def test_an_unreadable_matched_set_is_reported(tmp_path, capsys):
    game, csv = make_tree(tmp_path)
    (tmp_path / 'matched.json').write_text('{"version": 1}')
    assert run(tmp_path, game, csv) == 2
    assert 'cannot be read as a matched set' in capsys.readouterr().err


def test_a_constructor_not_recorded_exact_is_partial_not_vc6(tmp_path):
    header = HEADER.replace('\tstatic tTestShip* Instance();', '\ttTestShip();\n\tstatic tTestShip* Instance();')
    source = SOURCE + '\n// FUNCTION: STARFLEETOP 0x00401500\ntTestShip::tTestShip()\n{\n}\n'
    game, csv = make_tree(tmp_path, header=header, source=source)
    csv.write_text(CSV + '00401500,20,tTestShip::tTestShip,,,default,,,,0,,function,\n')
    write_matched(tmp_path, ('0x00401000',), partial=[('0x00401500', 20.0)])
    assert run(tmp_path, game, csv) == 0
    rows = {l.split('\t')[0]: l.split('\t')[4] for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}
    assert rows['0x00401500'] == 'partial'


# A global object whose class has a constructor and a destructor: VC6 generates its dynamic
# initializer, an atexit registration and an atexit destructor from the one definition line.
# Counted upwards from the definition: 0x00401500 initializer, 0x00401600 registration,
# 0x00401700 destructor. 0x00401700 is not in EXACT, so it is partial.
GLOBAL_SOURCE = SOURCE + (
    '\n// FUNCTION: STARFLEETOP 0x00401700\n'
    '// FUNCTION: STARFLEETOP 0x00401600\n'
    '// the comment of a hypothesis may sit among them\n'
    '// FUNCTION: STARFLEETOP 0x00401500\n'
    'tTestShip g_testShip;\n')
GLOBAL_CSV = CSV + ('00401500,18,FUN_00401500,,,default,,,,0,,function,\n'
                    '00401600,12,FUN_00401600,,,default,,,,0,,function,\n'
                    '00401700,20,FUN_00401700,,,default,,,,0,,function,\n')


def test_an_annotated_global_claims_the_functions_vc6_generates_from_it(tmp_path):
    game, csv = make_tree(tmp_path, source=GLOBAL_SOURCE)
    csv.write_text(GLOBAL_CSV)
    assert run(tmp_path, game, csv) == 0
    rows = {l.split('\t')[0]: l.split('\t') for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}
    assert rows['0x00401500'][1:] == ['18', 'g_testShip (dynamic initializer)', 'Test/TestShip.cpp', 'vc6']
    assert rows['0x00401600'][1:] == ['12', 'g_testShip (atexit registration)', 'Test/TestShip.cpp', 'vc6']
    assert rows['0x00401700'][1:] == ['20', 'g_testShip (atexit destructor)', 'Test/TestShip.cpp', 'partial']
    text = (tmp_path / 'out' / 'override.cpp').read_text()
    for a in ('00401500', '00401600', '00401700'):
        assert 'sub_%s' % a not in text and '0x%su' % a not in text
    assert 'g_testShip' not in text
    assert 'sfcop_guest_override_count = 4u;' in text


def test_a_stub_on_a_global_and_a_qualified_static_member_are_read(tmp_path):
    source = SOURCE + ('\n// STUB: STARFLEETOP 0x00401500\nstatic tTestShip g_spare[ 2 ];\n'
                       '\n// FUNCTION: STARFLEETOP 0x00401600\ntTestShip* tTestShip::sCurrent = 0;\n')
    game, csv = make_tree(tmp_path, source=source)
    csv.write_text(GLOBAL_CSV)
    functions, _ = manifest.build(str(game), str(csv), str(tmp_path / 'matched.json'))
    assert [(f['address'], manifest.symbol_of(f), f['modes']) for f in functions
            if f['address'] >= 0x00401500] == [
        (0x00401500, 'g_spare (dynamic initializer)', 'stub'),
        (0x00401600, 'tTestShip::sCurrent (dynamic initializer)', 'vc6')]


@pytest.mark.parametrize('definition,message', [
    ('extern tTestShip g_testShip;', "cannot read 'extern tTestShip g_testShip;' as a global"),
    ('tTestShip g_one, g_two;', "cannot read 'tTestShip g_one, g_two;' as a global"),
    ('class tTestShip;', "cannot read 'class tTestShip;' as a global"),
])
def test_a_declaration_that_is_not_one_global_definition_is_refused(tmp_path, capsys, definition, message):
    source = SOURCE + '\n// FUNCTION: STARFLEETOP 0x00401500\n%s\n' % definition
    game, csv = make_tree(tmp_path, source=source)
    csv.write_text(GLOBAL_CSV)
    assert run(tmp_path, game, csv) == 2
    assert message in capsys.readouterr().err


def test_a_data_member_is_not_a_global(tmp_path, capsys):
    header = HEADER.replace('\tint32 fSpeed;', '\t// FUNCTION: STARFLEETOP 0x00401500\n\tint32 fSpeed;')
    game, csv = make_tree(tmp_path, header=header)
    csv.write_text(GLOBAL_CSV)
    assert run(tmp_path, game, csv) == 2
    assert "TestShip.h:31: cannot read 'int32 fSpeed;' as a global" in capsys.readouterr().err


def test_more_annotations_than_vc6_generates_for_a_global_are_refused(tmp_path, capsys):
    source = GLOBAL_SOURCE.replace('// FUNCTION: STARFLEETOP 0x00401700\n',
                                   '// FUNCTION: STARFLEETOP 0x00401800\n// FUNCTION: STARFLEETOP 0x00401700\n')
    game, csv = make_tree(tmp_path, source=source)
    csv.write_text(GLOBAL_CSV + '00401800,10,FUN_00401800,,,default,,,,0,,function,\n')
    assert run(tmp_path, game, csv) == 2
    assert 'more than 3 annotations above the global g_testShip' in capsys.readouterr().err


def test_committed_manifest_and_override_table_match_the_tree():
    assert manifest.main(['--check']) == 0


# tTestShip's virtual destructor, defined out of class at 0x00401600, with its scalar deleting
# destructor (0x00401500) stacked above the destructor's own annotation, as the docstring's form
# says. functions.csv names 0x00401500 a scalar deleting destructor; 0x00401500 and 0x00401600
# are in EXACT.
DELETING_SOURCE = SOURCE + ('\n// FUNCTION: STARFLEETOP 0x00401500\n'
                            '// FUNCTION: STARFLEETOP 0x00401600\n'
                            'tTestShip::~tTestShip()\n{\n}\n')
DELETING_CSV = CSV + ('00401500,28,tTestShip::scalar_deleting_destructor,,,rtti-class-recovery,,,,0,,function,\n'
                      '00401600,12,tTestShip::~tTestShip,,,default,,,,0,,function,\n')


def manifest_rows(tmp_path):
    return {l.split('\t')[0]: l.split('\t')[1:]
            for l in (tmp_path / 'out' / 'manifest.tsv').read_text().splitlines()[1:]}


def test_a_deleting_destructor_stacked_on_the_destructor_is_named_for_what_it_is(tmp_path):
    game, csv = make_tree(tmp_path, source=DELETING_SOURCE)
    csv.write_text(DELETING_CSV)
    assert run(tmp_path, game, csv) == 0
    rows = manifest_rows(tmp_path)
    assert rows['0x00401500'] == ['28', "tTestShip::`scalar deleting destructor'", 'Test/TestShip.cpp', 'vc6']
    assert rows['0x00401600'] == ['12', 'tTestShip::~tTestShip()', 'Test/TestShip.cpp', 'vc6']
    text = (tmp_path / 'out' / 'override.cpp').read_text()
    assert 'sub_00401500' not in text and 'deleting destructor' not in text
    assert 'sfcop_guest_override_count = 4u;' in text


def test_a_deleting_destructor_alone_on_the_destructor_not_exact_is_partial(tmp_path):
    # Directly above the definition, with the destructor itself not claimed; the evidence is the
    # recorded symbol (??_E: vector) while functions.csv has only a placeholder name.
    source = SOURCE + '\n// FUNCTION: STARFLEETOP 0x00401500\ntTestShip::~tTestShip()\n{\n}\n'
    game, csv = make_tree(tmp_path, source=source)
    csv.write_text(CSV + '00401500,83,FUN_00401500,,,default,,,,0,,function,\n')
    (tmp_path / 'matched.json').write_text(
        '{"version": 1, "functions": {"0x00401500": {"exact": false, "score": 40.0, '
        '"symbol": "??_EtTestShip@@UAEPAXI@Z", "name": "FUN_00401500"}}}')
    assert run(tmp_path, game, csv) == 0
    assert manifest_rows(tmp_path)['0x00401500'] == [
        '83', "tTestShip::`vector deleting destructor'", 'Test/TestShip.cpp', 'partial']
    assert 'sub_00401500' not in (tmp_path / 'out' / 'override.cpp').read_text()


def test_a_deleting_destructor_claimed_on_a_stand_in_is_warned_about_and_kept(tmp_path, capsys):
    source = SOURCE.replace('// FUNCTION: STARFLEETOP 0x00401400', '// FUNCTION: STARFLEETOP 0x00401500')
    game, csv = make_tree(tmp_path, source=source)
    csv.write_text(DELETING_CSV)
    assert run(tmp_path, game, csv) == 0
    assert "0x00401500 is a scalar deleting destructor claimed on TestRound(int32)" in capsys.readouterr().err
    assert manifest_rows(tmp_path)['0x00401500'][1] == 'TestRound(int32)'


@pytest.mark.parametrize('header,source,message', [
    # Inside the class body, above an inline destructor.
    (HEADER.replace('\tvirtual ~tTestShip();',
                    '\t// FUNCTION: STARFLEETOP 0x00401500\n\tvirtual ~tTestShip() {}'),
     SOURCE, 'TestShip.h:18: an annotation inside the body of class tTestShip'),
    # Above the class.
    (HEADER.replace('// VTABLE: STARFLEETOP 0x00790020\n', '// FUNCTION: STARFLEETOP 0x00401500\n'),
     SOURCE, 'TestShip.h:13: an annotation above class tTestShip claims no function'),
    # A bare annotation: only a comment, then a blank line before the next definition.
    (HEADER, SOURCE.replace('// FUNCTION: STARFLEETOP 0x00401400\n',
                            '// FUNCTION: STARFLEETOP 0x00401500\n// generated by the compiler\n\n'
                            '// FUNCTION: STARFLEETOP 0x00401400\n'),
     'TestShip.cpp:26: the annotation is not followed by a definition'),
    # A declaration, not a definition.
    (HEADER, SOURCE + '\n// FUNCTION: STARFLEETOP 0x00401500\nvoid TestDeclared( int32 value );\n'
                      '\nvoid TestLater()\n{\n}\n',
     'TestShip.cpp:32: the annotation is not followed by a definition'),
    # At the end of the file.
    (HEADER, SOURCE + '\n// FUNCTION: STARFLEETOP 0x00401500\n',
     'TestShip.cpp:32: the annotation is not followed by a definition'),
])
def test_an_annotation_that_claims_no_definition_is_refused(tmp_path, capsys, header, source, message):
    game, csv = make_tree(tmp_path, header, source)
    csv.write_text(DELETING_CSV)
    assert run(tmp_path, game, csv) == 2
    assert message in capsys.readouterr().err


def test_a_member_declared_over_several_lines_is_read_as_one(tmp_path):
    header = HEADER.replace('\tvirtual void mFire( int32 count, const char* why );',
                            '\tvirtual void mFire( int32 count,\n\t\tconst char* why );')
    game, csv = make_tree(tmp_path, header=header)
    functions, classes = manifest.build(str(game), str(csv), str(tmp_path / 'matched.json'))
    fire = [m for m in classes['tTestShip']['members'] if m['name'] == 'mFire']
    assert [(m['types'], m['virtual'], m['line']) for m in fire] == [(('int32', 'const char*'), True, 20)]
    assert [f['name'] for f in functions if f['address'] == 0x00401200] == ['tTestShip::mFire']
