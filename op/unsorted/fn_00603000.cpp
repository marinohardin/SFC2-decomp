#include "Common/Thread/Optex.h"
#include "Tiger/tstring.h"
#include "unsorted/fn_0042a480_object.h"
#include "unsorted/fn_005430ad_owner.h"
#include "unsorted/fn_00571a58_owner.h"

// HYPOTHESIS: the 0x1c-byte element the function appends to the wrapped vector; not recovered.
class fn_00603c9a_entry
{
public:
	fn_00603c9a_entry( int a, int b, int c );

	int8u	field_00[0x1c];
};

// HYPOTHESIS: the list object at +0x13c; slot 44 returns its count, slot 59 the item at an index.
class fn_006037fc_list
{
public:
	virtual void fn_slot0();
	virtual void fn_slot1();
	virtual void fn_slot2();
	virtual void fn_slot3();
	virtual void fn_slot4();
	virtual void fn_slot5();
	virtual void fn_slot6();
	virtual void fn_slot7();
	virtual void fn_slot8();
	virtual void fn_slot9();
	virtual void fn_slot10();
	virtual void fn_slot11();
	virtual void fn_slot12();
	virtual void fn_slot13();
	virtual void fn_slot14();
	virtual void fn_slot15();
	virtual void fn_slot16();
	virtual void fn_slot17();
	virtual void fn_slot18();
	virtual void fn_slot19();
	virtual void fn_slot20();
	virtual void fn_slot21();
	virtual void fn_slot22();
	virtual void fn_slot23();
	virtual void fn_slot24();
	virtual void fn_slot25();
	virtual void fn_slot26();
	virtual void fn_slot27();
	virtual void fn_slot28();
	virtual void fn_slot29();
	virtual void fn_slot30();
	virtual void fn_slot31();
	virtual void fn_slot32();
	virtual void fn_slot33();
	virtual void fn_slot34();
	virtual void fn_slot35();
	virtual void fn_slot36();
	virtual void fn_slot37();
	virtual void fn_slot38();
	virtual void fn_slot39();
	virtual void fn_slot40();
	virtual void fn_slot41();
	virtual void fn_slot42();
	virtual void fn_slot43();
	virtual int fn_count();
	virtual void fn_slot45();
	virtual void fn_slot46();
	virtual void fn_slot47();
	virtual void fn_slot48();
	virtual void fn_slot49();
	virtual void fn_slot50();
	virtual void fn_slot51();
	virtual void fn_slot52();
	virtual void fn_slot53();
	virtual void fn_slot54( const tString& name, int a, int b, int index );
	virtual void fn_slot55( int index );
	virtual tString* fn_slot56( int index );
	virtual void fn_slot57( int index, int value );
	virtual void fn_slot58();
	virtual void* fn_item( int index );
};

// HYPOTHESIS: an object holding an optex at +0xb8 and a vector of owned pointers at +0x150;
// the owning class is not recovered.
class fn_006032e7_object
{
public:
	void fn_006032e7();
	void fn_005ff7ac();
	void** fn_006033e7( const db::tID& id );
	void** fn_00603486( const std::string& name );
	void fn_00603713( fn_0042a480_object* character );
	void fn_00603978( void* record );
	void fn_0060378d( int* message );
	int fn_006037fc( int* record );
	void fn_00603873( const std::string& name, int a, int b );
	void fn_00603575( int* record );
	void fn_00603a08( int index, int value );
	void fn_00603c9a( int a, int b, int c );

	int8u	field_00[0xb8];
	tOptex	field_b8;
	int8u	field_e4[0x13c - 0xb8 - sizeof( tOptex )];
	fn_006037fc_list*	field_13c;
	int8u	field_140[0x150 - 0x140];
	int32u	field_150;
	void**	field_154;
	void**	field_158;
	int8u	field_15c[0x178 - 0x15c];
	fn_0042c709_wrapper	field_178;
};

// HYPOTHESIS: a vector of owned pointers (begin at +4, end at +8); the class is not recovered.
class fn_00603351_vector
{
public:
	void fn_00603dee( void** it );
	void fn_00603e35();

	int32u	field_00;
	void**	field_04;
	void**	field_08;
};

// HYPOTHESIS: an object holding an optex at +0xb8; the owning class is not recovered.
class fn_00603351_object
{
public:
	void fn_00603351( fn_00603351_vector* list );

	int8u	field_00[0xb8];
	tOptex	field_b8;
};

// Inline in the SDK; Optex.h declares it only.
inline tOptex::tGuard::~tGuard()
{
	if (fNeedToLeave) {
		fOptex->Leave();
	}
}

// FUNCTION: STARFLEETOP 0x006032e7
void fn_006032e7_object::fn_006032e7()
{
	tOptex::tGuard guard( &field_b8 );
	for ( void** it = field_154; it != field_158; ++it )
	{
		if ( *it )
			delete *it;
	}
	( (fn_006032e7_object*)&field_150 )->fn_005ff7ac();
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00603351
void fn_00603351_object::fn_00603351( fn_00603351_vector* list )
{
	tOptex::tGuard guard( &field_b8 );
	for ( void** it = list->field_04; it != list->field_08; ++it )
	{
		if ( *it )
			delete *it;
	}
	list->fn_00603e35();
	guard.Leave();
}

// HYPOTHESIS: the character object (see fn_0042a480_object) extended with two counts at +0x140
// and +0x148; the owning class is not recovered.
class fn_006033b4_object : public fn_0042a480_object
{
public:
	int fn_006033b4( int* record );

	int8u	field_110[0x140 - sizeof( fn_0042a480_object )];
	int32u	field_140;
	int8u	field_144[4];
	int32u	field_148;
};

// FUNCTION: STARFLEETOP 0x006033b4
int fn_006033b4_object::fn_006033b4( int* record )
{
	int result = 0;
	switch ( *record )
	{
	case 2:
		// HYPOTHESIS: the record holds a character object at +8.
		result = ( (fn_0042a480_object*)( record + 2 ) )->FUN_0042aa2b();
		result += ( *(byte*)( record + 1 ) & 2 ) ? field_148 : field_140;
		break;
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x006033e7
void** fn_006032e7_object::fn_006033e7( const db::tID& id )
{
	tOptex::tGuard guard( &field_b8 );
	void** it = field_158;
	if ( id.IsValid() )
	{
		for ( it = field_154; it != field_158; ++it )
		{
			int* record = (int*)*it;
			if ( record && *record == 2 )
			{
				// HYPOTHESIS: the record holds a character object at +8, as in fn_006033b4.
				if ( ( (fn_0042a480_object*)( record + 2 ) )->FUN_0042a9ba() == id )
					break;
			}
		}
	}
	guard.Leave();
	return it;
}

// HYPOTHESIS: the original calls out-of-line instances of operator+ (0x005c8caf) and
// compare (0x0040651e); those have no shared header yet, so this uses the library's.
// FUNCTION: STARFLEETOP 0x00603486
void** fn_006032e7_object::fn_00603486( const std::string& name )
{
	tOptex::tGuard guard( &field_b8 );
	void** it = field_154;
	for ( ; it != field_158; ++it )
	{
		int* record = (int*)*it;
		if ( record && *record == 2 )
		{
			std::string joined = ( (fn_0042a480_object*)( record + 2 ) )->FUN_0042aa0b() + std::string( "" );
			if ( joined.compare( 0, joined.size(), name.c_str(), name.size() ) == 0 )
				break;
		}
	}
	guard.Leave();
	return it;
}

// HYPOTHESIS: the object at +8 of the entry (a tClientCharacter) is destroyed before the entry
// is freed; its destructor has no shared header yet, so only the free is written here.
// FUNCTION: STARFLEETOP 0x00603713
void fn_006032e7_object::fn_00603713( fn_0042a480_object* character )
{
	void** it = fn_006033e7( character->FUN_0042a9ba() );
	if ( it != field_158 )
	{
		void* entry = *it;
		( (fn_00603351_vector*)&field_150 )->fn_00603dee( it );
		fn_00603978( entry );
		if ( entry )
			delete entry;
	}
}

// FUNCTION: STARFLEETOP 0x0060378d
void fn_006032e7_object::fn_0060378d( int* message )
{
	int** it = (int**)field_158;
	switch ( message[0] )
	{
	case 2:
		it = (int**)fn_00603486( *(std::string*)( message + 3 ) );
		break;
	}
	if ( it != (int**)field_158 )
	{
		( *it )[1] = ( ~message[2] & ( *it )[1] ) | message[1];
		int found = fn_006037fc( *it );
		if ( found != -1 )
			fn_00603a08( found, ( (fn_006033b4_object*)this )->fn_006033b4( *it ) );
		else
			fn_00603575( *it + 2 );
	}
}

// FUNCTION: STARFLEETOP 0x006037fc
int fn_006032e7_object::fn_006037fc( int* record )
{
	tOptex::tGuard guard( &field_b8 );
	int index = 0;
	if ( field_13c )
	{
		int count = field_13c->fn_count();
		if ( count > 0 )
		{
			do
			{
				if ( field_13c->fn_item( index ) == record )
					goto done;
				++index;
			} while ( index < count );
		}
	}
	index = -1;
done:
	guard.Leave();
	return index;
}

// FUNCTION: STARFLEETOP 0x00603873
void fn_006032e7_object::fn_00603873( const std::string& name, int a, int b )
{
	tOptex::tGuard guard( &field_b8 );
	int index = 0;
	if ( field_13c )
	{
		int count = field_13c->fn_count();
		if ( count > 0 )
		{
			do
			{
				if ( field_13c->fn_item( index ) )
				{
					bool found = field_13c->fn_slot56( index )->mStrICmp( tString( name.c_str() ) ) > 0;
					if ( found )
						break;
				}
				++index;
			} while ( index < count );
		}
		field_13c->fn_slot54( tString( name.c_str() ), a, b, index );
	}
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00603978
void fn_006032e7_object::fn_00603978( void* record )
{
	tOptex::tGuard guard( &field_b8 );
	unsigned int index = 0;
	if ( field_13c )
	{
		unsigned int count = field_13c->fn_count();
		if ( count > 0 )
		{
			while ( 1 )
			{
				if ( field_13c->fn_item( index ) == record )
				{
					field_13c->fn_slot55( index );
					break;
				}
				++index;
				if ( index >= (unsigned int)field_13c->fn_count() )
					break;
			}
		}
	}
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00603a08
void fn_006032e7_object::fn_00603a08( int index, int value )
{
	tOptex::tGuard guard( &field_b8 );
	if ( field_13c )
		field_13c->fn_slot57( index, value );
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x00603c9a
void fn_006032e7_object::fn_00603c9a( int a, int b, int c )
{
	tOptex::tGuard guard( &field_b8 );
	{
		fn_0042c709_iptr lock = field_178.Lock();
		fn_00603c9a_entry* entry = new fn_00603c9a_entry( a, b, c );
		( (fn_00571a58_owner*)&*lock )->fn_00571a69( (int)&entry );
		lock.Done();
	}
	guard.Leave();
}

// HYPOTHESIS: a node holding a kind, two ints and a string; its name is not recovered.
class fn_00603d4a_node
{
public:
	fn_00603d4a_node( const std::string& text, int a, int b );

	int			m_kind;
	int			m_a;
	int			m_b;
	std::string	m_text;
};

// FUNCTION: STARFLEETOP 0x00603d4a
fn_00603d4a_node::fn_00603d4a_node( const std::string& text, int a, int b )
	: m_b( a ), m_kind( 2 ), m_a( b ), m_text( text )
{
}

// HYPOTHESIS: the vector the wrapper at +0x190 guards, seen through its member at 0x005541af;
// the owning class is not recovered.
class fn_005541af_vector
{
public:
	void fn_005541af( int arg );
};

class fn_00603d8f_object
{
public:
	void fn_00603d8f( int arg );

	int8u	field_00[0x190];
	fn_0042c709_wrapper	field_190;
};

// FUNCTION: STARFLEETOP 0x00603d8f
void fn_00603d8f_object::fn_00603d8f( int arg )
{
	fn_0042c709_iptr lock = field_190.Lock();
	( (fn_005541af_vector*)&*lock )->fn_005541af( arg );
	lock.Done();
}
