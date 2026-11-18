// Unit:      unsorted/fn_00436000.cpp
// Functions: 0x00436006 to 0x004364b6, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Common/BaseTypes.h"
#include "unsorted/tSafeArray.h"
#include "Tiger/taudiopriority.h"
#include "unsorted/tIndividualShipSystem.h"
#include "unsorted/tValueBase.h"

class tPath;
class twPath;
class tSession;
class tFSSession;
class tVolIterator;
class tVolIterParent;
class tDirIterator;
class tDirIterParent;
class tPathIterator;
class tPathIterParent;
class tVolume;
class twVolume;
class tDir;
class twDir;

#include "Tiger/tFSSession.h"
#include "Tiger/tCountMap.h"

// tCountMap, tValueCountMap and tCookieManager are declared in Tiger/tCountMap.h, as
// Tiger/tgfsys.cpp uses them too; the pair the maps hold needs the cookie and object types
// complete, which Tiger/tFSSession.h declares. These lines keep the line numbers.

// MATCH-ONLY: tBatterySystem's constructor (STARFLEETOP 0x004364b6) has tValueBase's
// constructor expanded inline and calls tNotifier's (0x004365cd) out of line, where
// tSupplyDockShipInfo's calls tValueBase's out-of-line copy (0x00436633). The constructor is
// declared out of line in unsorted/tValueBase.h and defined inline in this unit only.
inline tValueBase::tValueBase()
{
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

// FUNCTION: STARFLEETOP 0x004361e7
template <>
tCookieManager<tSession,tFSSession>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x00436212
template <>
tCookieManager<tVolIterator,tVolIterParent>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x0043623d
template <>
tCookieManager<tDirIterator,tDirIterParent>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x00436268
template <>
tCookieManager<tPathIterator,tPathIterParent>::tCookieManager()
{
	f_00 = 0;
}

// HYPOTHESIS: a singly linked list whose nodes have a virtual destructor and their link at
// +0x08; neither class is named by the exe.
class tNode_00436293
{
public:
	virtual ~tNode_00436293();

	int32u			f_04;
	tNode_00436293	*fNext;		// +0x08
};

class tList_00436293
{
public:
	void	FUN_00436293();

	int32u			f_00;
	tNode_00436293	*fHead;		// +0x04
	tNode_00436293	*fTail;		// +0x08
};

// FUNCTION: STARFLEETOP 0x00436293
void tList_00436293::FUN_00436293()
{
	tNode_00436293	*node = fHead;

	fHead = node->fNext;
	if ( fHead == 0 )
		fTail = 0;
	delete node;
}

// FUNCTION: STARFLEETOP 0x004362b0
template <>
tCookieManager<tVolume,twVolume>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x004362db
template <>
tCookieManager<tDir,twDir>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x00436306
template <>
tCookieManager<tPath,twPath>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x00436331
template <>
tCountMap<tSession,tFSSession,std::less<tSession> >::tCountMap()
{
	fHead = 0;
	fTail = 0;
}

// FUNCTION: STARFLEETOP 0x00436342
template <>
tCountMap<tVolIterator,tVolIterParent,std::less<tVolIterator> >::tCountMap()
{
	fHead = 0;
	fTail = 0;
}

// FUNCTION: STARFLEETOP 0x00436353
template <>
tCountMap<tDirIterator,tDirIterParent,std::less<tDirIterator> >::tCountMap()
{
	fHead = 0;
	fTail = 0;
}

// FUNCTION: STARFLEETOP 0x00436364
template <>
tCountMap<tPathIterator,tPathIterParent,std::less<tPathIterator> >::tCountMap()
{
	fHead = 0;
	fTail = 0;
}

// FUNCTION: STARFLEETOP 0x00436375
template <>
tCountMap<tVolume,twVolume,std::less<tVolume> >::tCountMap()
{
	fHead = 0;
	fTail = 0;
}

// FUNCTION: STARFLEETOP 0x00436386
template <>
tCountMap<tDir,twDir,std::less<tDir> >::tCountMap()
{
	fHead = 0;
	fTail = 0;
}

// FUNCTION: STARFLEETOP 0x00436397
template <>
tCountMap<tPath,twPath,std::less<tPath> >::tCountMap()
{
	fHead = 0;
	fTail = 0;
}

// HYPOTHESIS: the generic destructor is defined only inside op/Tiger/tAudioFile.cpp, so this unit
// writes the game's instance as a specialisation of the same body. The original tail-jumps to an
// out-of-line copy of mDeleteAll placed after this function.
// FUNCTION: STARFLEETOP 0x004363b3
// FUNCTION: STARFLEETOP 0x004363a8
template <>
tSafeArray<tClassTrackingInfo>::~tSafeArray()
{
	mDeleteAll();
}

// HYPOTHESIS: the handle check of the same array, out of line after mDeleteAll; the layout and
// the signed 16-bit use-count compare match tSafeArray's other inlined checks. No symbol names it.
// FUNCTION: STARFLEETOP 0x00436449
template <>
unsigned char tSafeArray<tClassTrackingInfo>::mIsThisIndexValid( unsigned long index )
{
	unsigned long realIndex = index & 0xffff;
	if ( realIndex >= f10 )
		return false;
	return (short)( index >> 16 ) == (short)f08[realIndex] && f04[realIndex] != NULL;
}

// HYPOTHESIS: the constructor's third argument; only the two ints at +4 and +8 are read.
struct fn_004364b6_param3
{
	int field_0;
	int field_4;
	int field_8;
};

// RTTI: tBatterySystem : tRepairableShipSystem (vtable 0x007993cc).
class tBatterySystem : public tRepairableShipSystem
{
public:
	// HYPOTHESIS: four parameters (ret 0x10), as tCloakSystem's; the fourth is unused here.
	tBatterySystem( int param_1, int param_2, fn_004364b6_param3 *param_3, int param_4 );

	// The base ends at +0x80 (its double at +0x48 rounds its size up to 8).
	tValue<long> field_80;
	tMinMaxFloat field_9c;
	int field_c0;
	bool field_c4;
};

// FUNCTION: STARFLEETOP 0x004364b6
tBatterySystem::tBatterySystem( int param_1, int param_2, fn_004364b6_param3 *param_3, int param_4 )
{
	field_78 = 0;
	field_28 = param_2;
	// HYPOTHESIS: param_1 is the owning ship.
	field_2c = (fn_owner_type*)param_1;
	field_0c = 0x4e;
	field_1c = 10;
	vfunction94( 0, -1 );
	// HYPOTHESIS: dispatched through the vftable, as in tCloakSystem's constructor.
	tIndividualShipSystem* system = this;
	system->vfunction49( param_3->field_8 - field_14 );
	system->vfunction47( param_3->field_4 - field_10 );
	field_80 = 3;
	tMinMaxFloat* charge = &field_9c;
	charge->vfunction3( 0.0f );
	charge->vfunction5( 0.0f );
	charge->vfunction4( (float)( field_14 >> 7 ) );
	field_c0 = 1;
	field_c4 = false;
	field_18 = fn_00453c60( "Battery", 10000, 0 );
}
