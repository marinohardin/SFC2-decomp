//	tState_Skirmish.h: tState_Skirmish, the skirmish setup state.
// Evidence:  class (RTTI; original file unknown)

#ifndef tState_Skirmish_h
#define tState_Skirmish_h 1

#include <vector>
#include "unsorted/tGameState.h"
#include "unsorted/tShipUI.h"
#include "unsorted/tReconnector.h"

// HYPOTHESIS: derives from tGameState (12 bytes: vtable, the list link, the number). The vtable
// at 0x0079f9f0 has 22 slots: tGameState's 19, which this class overrides at slots 1, 2, 3, 5, 12
// and 14 and inherits otherwise, and three of its own (slots 19 to 21). Slot names follow the
// disassembler's (slot N is vfunction(N+1), slot 0 the destructor).
// The members are placed by the offsets the constructor (0x0047f7b5) and the recovered bodies use.
// The object is a static (constructed at 0x00482a0e, pointer kept at 0x00915540), so no
// allocation size records its size: no size check is written. Members not seen are left as gaps.
// HYPOTHESIS: the object 0x0046b650 initializes; the original class is not known.
class fn_0046b650_Object
{
public:
	fn_0046b650_Object();
	~fn_0046b650_Object();	// 0x0046b6ac
	char			fUnknown_00[0x4c];
};

// HYPOTHESIS: 0x14 bytes at 0x1124 and 0x1138: a vector then a word the constructor zeroes;
// the original class is not known.
class fn_0047f7ab_VectorAndWord
{
public:
	fn_0047f7ab_VectorAndWord() : fWord( 0 ) {}
	std::vector<int>	fVector;
	long				fWord;
};

// HYPOTHESIS: the class of the object at 0x1154; only its method 0x00482eef is known.
class fn_00482eef_Object
{
public:
	long fn_00482eef( int a, int b );
};

class tState_Skirmish : public tGameState
{
public:
	tState_Skirmish();
	virtual ~tState_Skirmish();
	virtual bool vfunction2();					// slot 1, 0x0047fa30
	virtual void vfunction3();					// slot 2, 0x00480ba3
	virtual bool vfunction4();					// slot 3, 0x00480016; returns true
	virtual void vfunction6( int param_1 );	// slot 5, 0x004817a9
	// HYPOTHESIS: returns the state's name, "skirmish".
	virtual char *vfunction13();				// slot 12, 0x0047f7a5
	virtual void vfunction15( c_0046aa10_message *param_1 );	// slot 14, 0x004817b7
	virtual void vfunction20();					// slot 19, 0x00480152
	virtual void vfunction21();					// slot 20, 0x004801a5
	virtual void vfunction22();					// slot 21, 0x004800f2
	void fn_0047feb1();
	void fn_0047ff4e();
	// HYPOTHESIS: a method of the class of the object at 0x1154 (unknown); declared here until it is known.
	void fn_00482ed3( int a, int b, int c );
	void fn_0047ffe0();

	// HYPOTHESIS: 0x0c is the first word after tGameState's, not used in the bodies recovered; 0x10 is
	// the argument the helper at 0x004817ef gives the global manager's slot at +0x98.
	char			fUnknown_000c[0x4];
	long			field_0x10;
	// The ship display, constructed at 0x14 by 0x004cd034; the next member starts at 0x1118.
	tShipUI			field_0x14;
	// HYPOTHESIS: a char * compared with the literal "CustomSkmsh" by vfunction15.
	const char *	field_0x1118;
	long			field_0x111c;
	unsigned char	field_0x1120;
	unsigned char	field_0x1121;
	char			fUnknown_1122[0x2];
	// HYPOTHESIS: the constructor zeroes these as two empty vectors (an allocator byte then
	// three pointers), the first at 0x1124 and the second at 0x1138, each followed by a word.
	fn_0047f7ab_VectorAndWord	field_0x1124;
	fn_0047f7ab_VectorAndWord	field_0x1138;
	char			fUnknown_114c[0x8];
	// HYPOTHESIS: an object the destructor deletes through its virtual destructor.
	void *			field_0x1154;
	// A tReconnector (constructed by 0x0047f864 at 0x1158, 0x3c bytes); the rest up to the next member is not recovered.
	tReconnector	field_0x1158;
	char			fUnknown_1194[0x8];
	// HYPOTHESIS: a third std::vector, built the same way.
	std::vector<int>	field_0x119c;
	char			fUnknown_11ac[0xd0 - 0x3c - 0x8 - 0x10];
	// The object 0x0046b650 initializes (0x4c bytes at least).
	fn_0046b650_Object	field_0x1228;
	char			fUnknown_1274[0x28];
	unsigned char	field_0x129c;
};

#endif	// tState_Skirmish_h
