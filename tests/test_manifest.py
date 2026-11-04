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
    return game, csv


def run(tmp_path, game, csv, *extra):
    return manifest.main(['--game', str(game), '--functions', str(csv),
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
    _, classes = manifest.build(str(game), str(csv))
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
    (HEADER, SOURCE.replace('int32 TestRound( int32 value )', 'int32 __stdcall TestRound( int32 value )'),
     'calling convention __stdcall'),
    (HEADER.replace('class tTestShip : public tTestBase', 'class tTestShip : public tTestBase, public tOther'),
     SOURCE, 'multiple bases'),
])
def test_what_the_scanner_cannot_read_is_refused(tmp_path, capsys, header, source, message):
    game, csv = make_tree(tmp_path, header, source)
    assert run(tmp_path, game, csv) == 2
    assert message in capsys.readouterr().err


def test_signatures_are_read_from_definition_heads():
    assert manifest.parse_signature('int32 X::Y::operator==( const char* raw_name ) const') == (
        'int32', 'X::Y::operator==', ('const char*',), ('raw_name',), True)
    assert manifest.parse_signature('virtual int32 operator<( const tFoo& ) const') == (
        'int32', 'operator<', ('const tFoo&',), ('a0',), True)
    assert manifest.parse_signature('void tFoo::mSet( unsigned int, int32 count = 3 )') == (
        'void', 'tFoo::mSet', ('unsigned int', 'int32'), ('a0', 'count'), False)
    with pytest.raises(manifest.NotAFunction):
        manifest.parse_signature('ptr32( tFoo ) fNext')


def test_a_constructor_initialiser_list_is_not_part_of_the_signature(tmp_path):
    source = SOURCE.replace('// FUNCTION: STARFLEETOP 0x00401100\ntTestShip* tTestShip::Instance()',
                            '// FUNCTION: STARFLEETOP 0x00401100\ntTestShip* tTestShip::Instance() :\n'
                            '\tfSpeed( 0 )')
    game, csv = make_tree(tmp_path, source=source)
    functions, _ = manifest.build(str(game), str(csv))
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


def test_committed_manifest_and_override_table_match_the_tree():
    assert manifest.main(['--check']) == 0
