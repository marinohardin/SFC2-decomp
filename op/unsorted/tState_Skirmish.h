//	tState_Skirmish.h: tState_Skirmish, the skirmish setup state.
// Evidence:  class (RTTI; original file unknown)

#ifndef tState_Skirmish_h
#define tState_Skirmish_h 1

#include "unsorted/tGameState.h"

// HYPOTHESIS: derives from tGameState (12 bytes: vtable, the list link, the number). The vtable
// at 0x0079f9f0 has 22 slots; every slot is declared, in the exe's order, so that the three
// this class adds (slots 19 to 21) land where the exe has them. Slot names follow the
// disassembler's (slot N is vfunction(N+1), slot 0 the destructor); types are given only where
// a body shows them, the rest are a guess from the shared stub the slot points at.
// The members are placed by the offsets the constructor (0x0047f7b5) and the recovered bodies use.
// The object is a static (constructed at 0x00482a0e, pointer kept at 0x00915540), so no
// allocation size records its size: no size check is written. Members not seen are left as gaps.
class tState_Skirmish : public tGameState
{
public:
	tState_Skirmish();
	virtual ~tState_Skirmish();
	virtual bool vfunction2();					// slot 1, 0x0047fa30
	virtual void vfunction3();					// slot 2, 0x00480ba3
	virtual bool vfunction4();					// slot 3, 0x00480016; returns true
	virtual void vfunction5();					// slot 4: tGameState's shared stub
	virtual void vfunction6();					// slot 5, 0x004817a9
	virtual void vfunction7();					// slot 6: the shared stub
	virtual void vfunction8( int *param_1 );	// slot 7: tGameState's (0x00469ddd)
	virtual void vfunction9();					// slot 8: the shared stub
	virtual void vfunction10();					// slot 9: the shared stub
	virtual void vfunction11();					// slot 10: the shared stub
	virtual char *vfunction12();				// slot 11: tGameState's (0x005e8efe)
	// HYPOTHESIS: returns the state's name, "skirmish".
	virtual char *vfunction13();				// slot 12, 0x0047f7a5
	virtual bool vfunction14( int param_1 );	// slot 13: tGameState's (0x00469dfd)
	virtual void vfunction15( int *param_1 );	// slot 14, 0x004817b7
	virtual void vfunction16( int *param_1 );	// slot 15: tGameState's (0x00469e02)
	virtual void vfunction17();					// slot 16: the shared stub
	virtual void vfunction18();					// slot 17: tGameState's (0x0046ad4d)
	virtual void vfunction19();					// slot 18: 0x00402fa5, shared with other states
	virtual void vfunction20();					// slot 19, 0x00480152
	virtual void vfunction21();					// slot 20, 0x004801a5
	virtual void vfunction22();					// slot 21, 0x004800f2

	// HYPOTHESIS: 0x0c is the first word after tGameState's, not used in the bodies recovered; 0x10 is
	// the argument the helper at 0x004817ef gives the global manager's slot at +0x98.
	char			fUnknown_000c[0x4];
	long			field_0x10;
	// The ship display, constructed at 0x14 by 0x004cd034; its last store (a flag) is at 0x1100
	// of it, and the next member starts at 0x1118.
	char			fUnknown_0014[0x1104];
	// HYPOTHESIS: a char * compared with the literal "CustomSkmsh" by vfunction15.
	const char *	field_0x1118;
	long			field_0x111c;
	unsigned char	field_0x1120;
	unsigned char	field_0x1121;
	char			fUnknown_1122[0x2];
	// HYPOTHESIS: the constructor zeroes these as two empty vectors (an allocator byte then
	// three pointers), the first at 0x1124 and the second at 0x1138, each followed by a word.
	char			fUnknown_1124[0x10];
	long			field_0x1134;
	char			fUnknown_1138[0x10];
	long			field_0x1148;
	char			fUnknown_114c[0x8];
	// HYPOTHESIS: an object the destructor deletes through its virtual destructor.
	void *			field_0x1154;
	// A tReconnector (0x0047f864), up to the next member; its size is not recovered.
	char			fUnknown_1158[0xd0];
	// The object 0x0046b650 initializes (0x4c bytes at least).
	char			fUnknown_1228[0x4c];
	char			fUnknown_1274[0x38];
	unsigned char	field_0x12ac;
};

#endif	// tState_Skirmish_h
