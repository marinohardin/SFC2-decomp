#include "Common/Thread/Optex.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/fn_004D3143.h"
#include "unsorted/tVisObject.h"
#include "unsorted/tMetaMapControl.h"

// MATCH-ONLY: inline in the SDK, as in unsorted/tClientCharacter.cpp, which claims its
// out-of-line copy (0x00429a18); the header cannot define it while that unit does.
inline tOptex::tGuard::~tGuard()
{
	if( fNeedToLeave )
	{
		fOptex->Leave();
	}
}

// HYPOTHESIS: the claim at 0x004d353e is declared void, but this caller casts its result; it
// returns what 0x004d4728 finds for a group and a control name.
void fn_004d353e( long arg1, long arg2 );

// HYPOTHESIS: the name of the map panel's control group, a global at 0x0087632c.
extern const char* g_0087632c;
// HYPOTHESIS: only the members read here are placed; the class at 0x1b8 and its argument are not recovered.
class tMetaClientMapSub { public: void fn_005e94ef( long arg ); void fn_005e9499(); unsigned char field_0[ 0xc ]; void* field_c; long field_10; };
// HYPOTHESIS: a cursor over the list at 0x1b8 (node pointer, list pointer) and the receiver of its result.
class tMetaClientMapCursor { public: long fn_005e9cb0(); void* fNode; tMetaClientMapSub* fList; };
class tMetaClientMapReceiver { public: void fn_005e5ba6( long arg ); };
class tMetaClientMapPanel
{
public:
	virtual void vfunction19();
	virtual void vfunction20();
	int fn_005e5957( unsigned long flag ); bool fn_005e5a8f(); void fn_005e5abe(); void fn_005e5ae0( long arg ); bool fn_005e5b2d( tMetaClientMapReceiver* receiver );
	void fn_005e6091();

	unsigned char field_4[ 0xdc - 0x4 ];
	tOptex field_dc;
	unsigned char field_f0[ 0xf8 - 0xdc - sizeof( tOptex ) ];
	tMetaMapControl* fpMetaMapControl;
	unsigned char field_fc[ 0x18c - 0xfc ]; bool field_18c; unsigned char field_18d[ 0x1a0 - 0x18d ];
	int field_1a0; unsigned char field_1a4[ 0x1b8 - 0x1a4 ]; tMetaClientMapSub field_1b8;
};

// FUNCTION: STARFLEETOP 0x005e5800
void tMetaClientMapPanel::vfunction19()
{
	tOptex::tGuard guard( &field_dc );

	// MATCH-ONLY: the claim at 0x004d353e is declared void; its result is reached through a cast.
	fpMetaMapControl = dynamic_cast<tMetaMapControl*>( ( (tVisObject* (*)( long, long )) fn_004d353e )( (long) g_0087632c, (long) "FakeMap" ) );
	if( fpMetaMapControl == NULL )
	{
		fn_005e6091();
		fpMetaMapControl = dynamic_cast<tMetaMapControl*>( ( (tVisObject* (*)( long, long )) fn_004d353e )( (long) g_0087632c, (long) "FakeMap" ) );
#line 316
		validate( fpMetaMapControl );
#line 57
	}
	fpMetaMapControl->fn_005e3148( field_1a0 );
	fpMetaMapControl->fn_005e3ad8();
	fn_004d3331( g_0087632c );
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005e58f6
void tMetaClientMapPanel::vfunction20()
{
	tOptex::tGuard guard( &field_dc );

	fpMetaMapControl->fn_005e3b6e();
	fpMetaMapControl->fn_005e31be();
	fn_004d3340( g_0087632c );
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005e5957
int tMetaClientMapPanel::fn_005e5957( unsigned long flag )
{
	field_dc.Enter();

	int result;
	switch( flag )
	{
		case 0x2:		result = 1;		break;
		case 0x4:		result = 2;		break;
		case 0x8:		result = 3;		break;
		case 0x10:		result = 4;		break;
		case 0x20:		result = 5;		break;
		case 0x40:		result = 6;		break;
		case 0x80:		result = 7;		break;
		case 0x100:		result = 8;		break;
		case 0x200:		result = 6;		break;
		case 0x400:		result = 6;		break;
		case 0x800:		result = 6;		break;
		case 0x1000:	result = 12;	break;
		case 0x2000:	result = 13;	break;
		case 0x4000:	result = 14;	break;
		case 0x8000:	result = 15;	break;
		case 0x10000:	result = 16;	break;
		case 0x20000:	result = 17;	break;
		case 0x40000:	result = 18;	break;
		case 0x80000:	result = 19;	break;
		case 0x100000:	result = 20;	break;
		case 0x200000:	result = 21;	break;
		case 0x400000:	result = 22;	break;
		case 0x800000:	result = 23;	break;
		default:		result = 0;		break;
	}
	field_dc.Leave();
	return result;
}

// FUNCTION: STARFLEETOP 0x005e5a8f
bool tMetaClientMapPanel::fn_005e5a8f()
{
	field_dc.Enter();

	bool flagged = false;
	if( field_18c )
	{
		field_18c = false;
		flagged = true;
	}
	field_dc.Leave();
	return flagged;
}

// FUNCTION: STARFLEETOP 0x005e5abe
void tMetaClientMapPanel::fn_005e5abe()
{
	field_dc.Enter();
	field_18c = true;
	field_dc.Leave();
}

// FUNCTION: STARFLEETOP 0x005e5ae0
void tMetaClientMapPanel::fn_005e5ae0( long arg )
{
	tOptex::tGuard guard( &field_dc );

	field_1b8.fn_005e94ef( arg );
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005e5b2d
bool tMetaClientMapPanel::fn_005e5b2d( tMetaClientMapReceiver* receiver )
{
	tOptex::tGuard guard( &field_dc );

	bool found = false;
	if( field_1b8.field_10 != 0 )
	{
		tMetaClientMapCursor cursor;
		cursor.fNode = field_1b8.field_c;
		cursor.fList = &field_1b8;
		receiver->fn_005e5ba6( cursor.fn_005e9cb0() );
		field_1b8.fn_005e9499();
		found = true;
	}
	guard.Leave();
	return found;
}
