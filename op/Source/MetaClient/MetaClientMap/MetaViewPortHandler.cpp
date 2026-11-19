#include "Common/Thread/Optex.h"
#include "Common/Exceptions/Validate.h"
#include "unsorted/fn_004D3143.h"
#include "unsorted/tVisObject.h"
#include "unsorted/tMetaMapControl.h"
#include "unsorted/tHexPoint.h"
#include "Common/Thread/SingleAccessWrapper.h"
#include "unsorted/tObject_0092deb4.h"
#include "unsorted/fn_0042a480_object.h"
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
// HYPOTHESIS: only the members read here are placed; the class at 0x1b8 and its argument are not recovered. The member at 0xfc is some container; only its 0x54b6cf method is placed. Its +0x24 takes the panel's new value (0x005e8e55); that object's size is unknown.
class tMetaClientMapQueueMember { public: void fn_005e8e55( int arg ); unsigned char field_0; }; class tMetaClientMapSub { public: void fn_005e94ef( long arg ); void fn_005e9499(); unsigned char field_0[ 0xc ]; void* field_c; long field_10; }; class tMetaClientMapQueue { public: void fn_0054b6cf( long arg ); const tHexPoint& fn_0058c2e1(); int fn_005e8efa(); int fn_00423402(); unsigned char field_0[ 0x24 ]; tMetaClientMapQueueMember field_24; unsigned char field_25[ 0x18c - 0xfc - 0x25 ]; };
// HYPOTHESIS: a cursor over the list at 0x1b8 (node pointer, list pointer) and the receiver of its result.
class tMetaClientMapCursor { public: long fn_005e9cb0(); void* fNode; tMetaClientMapSub* fList; };
class tMetaClientMapMember { public: void fn_005b69b0( void* source ); unsigned char field_0[ 0x14 ]; }; class tMetaClientMapReceiver { public: tMetaClientMapReceiver* fn_005e5ba6( long arg ); long field_0; tMetaClientMapMember field_4; tMetaClientMapMember field_18; long field_2c; };
class tMetaClientMapPanel
{
public:
	virtual void vfunction19();
	virtual void vfunction20();
	int fn_005e5957( unsigned long flag ); bool fn_005e5a8f(); void fn_005e5abe(); void fn_005e5ae0( long arg ); bool fn_005e5b2d( tMetaClientMapReceiver* receiver ); void fn_005e5bd7( long arg ); void fn_005e5bfe( long arg ); void fn_005e5c25( long arg ); void fn_005e5c72( const tHexPoint& position ); tHexPoint fn_005e5ced(); tHexPoint fn_005e5d7a(); long fn_005e5dde(); long fn_005e5e01(); void fn_005e5e24( int arg ); void fn_005e5edc(); void fn_005e8c29( const tRect& area );
	void fn_005e6091();
	unsigned char field_4[ 0xdc - 0x4 ];
	tOptex field_dc;
	unsigned char field_f0[ 0xf8 - 0xdc - sizeof( tOptex ) ];
	tMetaMapControl* fpMetaMapControl;
	tMetaClientMapQueue field_fc; bool field_18c; unsigned char field_18d[ 0x190 - 0x18d ]; long field_190; long field_194; unsigned char field_198[ 0x1a0 - 0x198 ];
	int field_1a0; unsigned char field_1a4[ 0x1b8 - 0x1a4 ]; tMetaClientMapSub field_1b8; unsigned char field_1cc[ 0x1d0 - 0x1cc ]; tHexPoint field_1d0;
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

// FUNCTION: STARFLEETOP 0x005e5ba6
tMetaClientMapReceiver* tMetaClientMapReceiver::fn_005e5ba6( long arg )
{
	long* record = (long*) arg;
	field_0 = record[ 0 ];
	field_4.fn_005b69b0( record + 1 );
	field_18.fn_005b69b0( record + 6 );
	field_2c = record[ 11 ];
	return this;
}

// FUNCTION: STARFLEETOP 0x005e5bd7
void tMetaClientMapPanel::fn_005e5bd7( long arg )
{
	field_dc.Enter();
	field_190 = arg;
	field_dc.Leave();
}

// FUNCTION: STARFLEETOP 0x005e5bfe
void tMetaClientMapPanel::fn_005e5bfe( long arg )
{
	field_dc.Enter();
	field_194 = arg;
	field_dc.Leave();
}

// FUNCTION: STARFLEETOP 0x005e5c25
void tMetaClientMapPanel::fn_005e5c25( long arg )
{
	tOptex::tGuard guard( &field_dc );

	field_fc.fn_0054b6cf( arg );
	guard.Leave();
}

// FUNCTION: STARFLEETOP 0x005e5c72
void tMetaClientMapPanel::fn_005e5c72( const tHexPoint& position )
{
	fn_005e5c25( (long) &tHexPoint( position.fn_0055f269() - field_fc.fn_00423402() / 2, position.fn_005e8f21() - field_fc.fn_005e8efa() / 2 ) );
}

// FUNCTION: STARFLEETOP 0x005e5ced
tHexPoint tMetaClientMapPanel::fn_005e5ced()
{
	tHexPoint corner( field_fc.fn_0058c2e1() );
	return tHexPoint( corner.fn_0055f269() + field_fc.fn_00423402() / 2, corner.fn_005e8f21() + field_fc.fn_005e8efa() / 2 );
}

// MATCH-ONLY: the SDK's inline tOptex destructor, the body tClientCharacter.cpp claims as the
// game's copy (0x004296c8), defined out of line here. In the original file the panel's earlier
// functions (its destructor 0x005e506f and the members up to 0x005e57b2, another unit for now)
// had already compiled it, so the compiler knew it cannot throw and set no unwind state before
// calling it from the inlined tAccessPointer destructor. Defined inline here, it is compiled
// only after this function and the call gets an extra state store.
tOptex::~tOptex()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections && fpSharedInfo != NULL)
			::DeleteCriticalSection(&fpSharedInfo->fCriticalSection);
		if (fEvent != NULL)
			::CloseHandle(fEvent);
		if (fFileMapping == NULL) {
			delete fpSharedInfo;
		} else {
			::UnmapViewOfFile(fpSharedInfo);
			::CloseHandle(fFileMapping);
		}
	}
}

// FUNCTION: STARFLEETOP 0x005e5d7a
tHexPoint tMetaClientMapPanel::fn_005e5d7a()
{
	return ( *gObject_0092deb4->fn_004d16b2() ).FUN_0042abe4();
}

// FUNCTION: STARFLEETOP 0x005e5dde
long tMetaClientMapPanel::fn_005e5dde()
{
	field_dc.Enter();
	long result = field_190;
	field_dc.Leave();
	return result;
}

// FUNCTION: STARFLEETOP 0x005e5e01
long tMetaClientMapPanel::fn_005e5e01()
{
	field_dc.Enter();
	long result = field_194;
	field_dc.Leave();
	return result;
}

// MATCH-ONLY: inline in the SDK; Optex.h declares it only. Its out-of-line copy (0x004297fa) is
// claimed in tClientCharacter.cpp. With the body in view the compiler knows Leave cannot throw (it
// calls only C functions), so the guard's release below sets no unwind state first. In the
// original file the body was in view from the top; here that would change the functions above.
inline void tOptex::Leave()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections) {
			::LeaveCriticalSection(&fpSharedInfo->fCriticalSection);
		} else if (--fpSharedInfo->fRecurseCount > 0) {
			::InterlockedDecrement(&fpSharedInfo->fLockCount);
		} else {
			fpSharedInfo->fThreadId = 0;
			if (::InterlockedDecrement(&fpSharedInfo->fLockCount) > 0)
				::SetEvent(fEvent);
		}
	}
}

// FUNCTION: STARFLEETOP 0x005e5e24
void tMetaClientMapPanel::fn_005e5e24( int arg )
{
	tOptex::tGuard guard( &field_dc );

	field_1a0 = arg;
	tHexPoint center = fn_005e5ced();
	field_fc.field_24.fn_005e8e55( arg );
	fn_005e8c29( tRect( 0, 0, 522, 342 ) );
	if( field_1d0.fn_005b6a5c( 0, 0 ) )
		fn_005e5c72( field_1d0 );
	else
		fn_005e5c72( center );
	fn_005e5edc();
}

// FUNCTION: STARFLEETOP 0x005e5edc
void tMetaClientMapPanel::fn_005e5edc()
{
	fpMetaMapControl->fn_005e4468();
}
