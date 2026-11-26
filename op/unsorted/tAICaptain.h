//	tAICaptain.h: tAICaptain, the AI that commands one ship, and the classes its layout names.
// Evidence:  class (RTTI; original file unknown), size 0x3a48 (the allocation at 0x005228f2)

#ifndef tAICaptain_h
#define tAICaptain_h 1

#include <string.h>
#include <list>
#include <vector>
#include "tEntity.h"
#include "tAIOfficer.h"
#include "tAIHelmOfficer.h"
#include "tAIOffensiveOfficer.h"
#include "tAIDefensiveOfficer.h"
#include "unsorted/tObject_0092deb4.h"
#include "tObject_0096eca8.h"
#include "fn_0090be9c_type.h"
#include "tShipCommand.h"
// HYPOTHESIS: the second base of the officers that ask for extra energy (RTTI); only its position in each officer is known, from the pointers tAICaptain's constructor hands to its power object.
class tExtraEnergyRequest
{
public:
	virtual void vfunction1();
};
class tObject_00528a11;
#include "unsorted/tObject_005237dc.h"











// HYPOTHESIS: the object at 0x35c8 of tAICaptain, 0x24 bytes, called the selector where the
// officers read it; its constructor (0x00528a11) takes nothing and it has no destructor. A
// second constructor (0x00528a23) takes five values; 0x00514f8b copies all 0x24 bytes.
class tObject_00528a11
{
public:
	tObject_00528a11();
	tObject_00528a11(int param_1, int param_2, int param_3, int param_4, char param_5);
	int field_0x0;                          // +0x0
	// HYPOTHESIS: the two values 0x0051e454 passes to the helm officer's 0x005169e0.
	int field_0x4;                          // +0x4
	int field_0x8;                          // +0x8
	int field_0xc;                          // +0xc
	int field_0x10;                         // +0x10
	char field_0x14[0xc];                   // +0x14
	char field_0x20;                        // +0x20
	char field_0x21[0x3];                   // +0x21
};
class tObject_004b94dc;
class tObject_00524875;
// HYPOTHESIS: the object the pointer at 0x0090bed4 names; tSfcApp3d.cpp declares the same
// pointer with only its method 0x00522681. Its method 0x00522708 returns an object tAICaptain
// keeps at 0x35fc.
class tObject_00522708
{
public:
	tObject_00524875 *fn_00522708();
	tObject_004b94dc *fn_00523574(int aID);
	// HYPOTHESIS: takes the object 0x00522708 returned; tAICaptain's destructor hands it back.
	void fn_00522681(tObject_00524875 *aObject);
};
class tObject_0051e0c4_list;
class tObject_0051e0c4_35b8;
class tObject_0051e0c4_3a34;
extern tObject_00522708 *g_0090bed4;
// HYPOTHESIS: a double tAICaptain's constructor copies to 0xb0.
extern double g_0090bef0;
// HYPOTHESIS: a string tAICaptain's constructor passes with its second argument to 0x0054814e.
extern const char g_0085912c[];
// HYPOTHESIS: the officer classes below are declared only as far as tAICaptain's constructor uses
// them: their constructors take the ship ID, and those that also ask for extra energy have that
// base after their tAIOfficer part (0x14 bytes), or after tAITransportOfficer's (0x24 bytes).
class tAITransportOfficer : public tAIOfficer
{
public:
	tAITransportOfficer(int aShipID);
	char field_0x14[0x10];
};
class tAIShieldReinforcementOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAIShieldReinforcementOfficer(int aShipID);
};
class tAIECMOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAIECMOfficer(int aShipID);
};
class tAIECCMOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAIECCMOfficer(int aShipID);
};
class tAITractorOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAITractorOfficer(int aShipID);
};
class tAIRepairOfficer : public tAIOfficer
{
public:
	tAIRepairOfficer(int aShipID);
};
class tAISeekingWeaponDefenseOfficer : public tAIOfficer, public tExtraEnergyRequest
{
public:
	tAISeekingWeaponDefenseOfficer(int aShipID);
};
class tAIHitAndRunOfficer : public tAITransportOfficer, public tExtraEnergyRequest
{
public:
	tAIHitAndRunOfficer(int aShipID);
};
class tAITBombOfficer : public tAITransportOfficer, public tExtraEnergyRequest
{
public:
	tAITBombOfficer(int aShipID);
};
class tAIHETOfficer : public tAIOfficer
{
public:
	tAIHETOfficer(int aShipID);
	// HYPOTHESIS: whether the officer takes over; 0x0051e613 then skips the selector search.
	bool fn_0051819f();
};
class tAICloakOfficer : public tAIOfficer
{
public:
	tAICloakOfficer(int aShipID, tAIHelmOfficer *aHelm, tObject_005237dc *aPower);
	void fn_00514bea(int aValue); // HYPOTHESIS: takes one value; tAICaptain's 0x0051f16e forwards its argument.
};
class tAIFighterOfficer : public tAIOfficer
{
public:
	tAIFighterOfficer(int aShipID);
};
class tAIPlanetKillerOfficer : public tAIOfficer
{
public:
	tAIPlanetKillerOfficer(int aShipID);
};
class tAIShuttleOfficer : public tAIOfficer
{
public:
	tAIShuttleOfficer(int aShipID);
};
// HYPOTHESIS: layout unknown beyond the constructor; filled from the disassembly. The four arguments: a float kept at 0x3a30 and given to every officer, a value passed on to 0x0054814e, one the constructor does not use, and the ship ID.
class tAICaptain : public tEntity
{
public:
	tAICaptain(float aParam1, int aParam2, int aParam3, int aShipID);
	virtual ~tAICaptain();
	virtual void vfunction7(tObject_0041018e_arg *param_1, int param_2);
	virtual int vfunction18();
	virtual void vfunction3();
	virtual void vfunction4(tObject_00413ac3_arg *aObject);
	void fn_0051e275(int aValue);
	void fn_0051f559();
	tObject_0051e0c4_35b8 *fn_0051e32e();
	void fn_0051e33f(tShipCommand *param_1); // HYPOTHESIS: takes a command for the ship; 0x0051f0e1 passes a tCommandDisengage or a tCommandClearCurrentOrders.
	tPoint3 fn_0051e37d();
	void fn_0051e3a1();
	bool fn_0051e3ac(int aID, int aValue);
	tObject_0051e0c4_3a34 *fn_0051e44d();
	void fn_0051fa75();
	void fn_0051fa3c();
	void fn_0051fce4();
	tPoint3 fn_0051feca(tObject_0042496f *aShip); // HYPOTHESIS: returns by value a point for the ship given; 0x0051fa3c sends the ship there.
	void fn_0051ff8b(int aTargetID, char aFlag); // HYPOTHESIS: sets the target ID at 0xc4 and the flag at 0xd0, and hands the ID to the helm officer.
	void fn_00520004(float aValue, unsigned char aCount, char *aData); // HYPOTHESIS: a float, a count and that many bytes, as order 2 carries them.
	void fn_0051f1ed();
	void fn_0052008f();
	void fn_0051f602();
	void fn_0051e613();
	void fn_0051f0c3(char aValue);
	char fn_0051f0d0();
	void fn_0051f0e1(char aValue);
	void fn_0051f153(int aIndex, int aValue);
	void fn_0051f16e(int aValue);
	void fn_0051f184(tObject_00525081 *aFormation);
	void fn_0051ffae(tPoint3 aPoint);
	void fn_0051ff8b(int aID, int aValue);
	void fn_0051f958(int aID, char aFlag);
	void fn_0051f9d1(int aID);
	void fn_0051f9e9(int aID);
	void fn_0051ffa0(int aID, char aFlag); // HYPOTHESIS: argument types; 0x0051f9d1 passes its argument and 1.
	tAICaptain *scalar_deleting_destructor(unsigned char flags);
	void fn_0054814e(int param_1, const char *param_2); // HYPOTHESIS: 0x0054814e is three bytes, so an empty function that takes two arguments.
	int field_0x88;                  // +0x88
	int field_0x8c;                         // +0x8c
	char field_0x90;                        // +0x90
	// HYPOTHESIS: a list (the constructor sets allocator, head node, count); element type unknown.
	std::list<int> field_0x94;              // +0x94
	double field_0xa0;                      // +0xa0
	double field_0xa8;                      // +0xa8
	double field_0xb0;                      // +0xb0
	char field_0xb8[0xc];                   // +0xb8
	int field_0xc4;                         // +0xc4
	int field_0xc8;                         // +0xc8
	int field_0xcc;                         // +0xcc HYPOTHESIS: the ID 0x0051f9e9 hands the tractor officer
	char field_0xd0;                        // +0xd0
	char field_0xd1[0x3];                   // +0xd1
	tObject_005237dc field_0xd4;            // +0xd4
	int field_0x35b4;                       // +0x35b4
	tObject_0051e0c4_35b8 *field_0x35b8;    // +0x35b8
	tObject_0051e0c4_35b8 *field_0x35bc;    // +0x35bc
	int field_0x35c0;                       // +0x35c0
	int field_0x35c4;                       // +0x35c4
	tObject_00528a11 field_0x35c8;          // +0x35c8
	float field_0x35ec;                     // +0x35ec
	float field_0x35f0;                     // +0x35f0
	float field_0x35f4;                     // +0x35f4
	char field_0x35f8;                      // +0x35f8
	tObject_00524875 *field_0x35fc;         // +0x35fc
	char field_0x3600;                      // +0x3600
	short field_0x3602;                     // +0x3602
	float field_0x3604;                     // +0x3604
	tAIOffensiveOfficer field_0x3608;       // +0x3608
	tAIDefensiveOfficer field_0x37a8;       // +0x37a8
	tAIHelmOfficer field_0x3810;            // +0x3810
	tAIOfficer *field_0x396c[13];           // +0x396c
	float field_0x39a0;                     // +0x39a0 HYPOTHESIS: a range 0x0051f602 searches in
	unsigned char field_0x39a4;             // +0x39a4
	unsigned char field_0x39a5;             // +0x39a5
	char field_0x39a6[0x78];                // +0x39a6
	char field_0x3a1e;                      // +0x3a1e
	char field_0x3a1f[0x11];                // +0x3a1f
	float field_0x3a30;                     // +0x3a30
	tObject_0051e0c4_3a34 *field_0x3a34;    // +0x3a34
	tObject_00525081 *field_0x3a38;         // +0x3a38 HYPOTHESIS: the formation, as the helm's 0x140
	char field_0x3a3c;                      // +0x3a3c
	char field_0x3a3d;                      // +0x3a3d
	char field_0x3a3e;                      // +0x3a3e
	unsigned int field_0x3a40;              // +0x3a40 HYPOTHESIS: a time; 0x0051e454 reads it unsigned
	char field_0x3a44;                      // +0x3a44
	char field_0x3a45;                      // +0x3a45
};
typedef char tAICaptain_size_check[ sizeof( tAICaptain ) == 0x3a48 ? 1 : -1 ];

#endif	// tAICaptain_h
