//	tState_MetaLogon.h: tState_MetaLogon, the state that logs on to the Dynaverse.
// Evidence:  class (RTTI; vtables 0x0079f284 and 0x0079f27c), the constructor 0x0046d0b0 and the
// destructor 0x0046d1c8

#ifndef tState_MetaLogon_h
#define tState_MetaLogon_h 1

#include <string>
#include <vector>
#include "Tiger/tstring.h"
#include "unsorted/tValueI.h"
#include "unsorted/tGameState.h"
#include "unsorted/tStatusAccess.h"
#include "unsorted/fn_005717f7_vector.h"

// The RTTI names two bases, tGameState (at +0) and tStatusAccess (at +0xc, vtable 0x0079f27c).
// The single global instance is at 0x0090e250 (constructed by 0x0046d090, destroyed by
// 0x0046d0a6); its size is where the last member ends, 0x1a0. Members are placed by what the
// constructor initialises and by the offsets the methods use.
//
// The vtable at 0x0079f284 overrides slot 0 (the deleting destructor 0x0046d1ac) and slots 1, 2,
// 3 and 12; every other slot keeps tGameState's. The virtual functions are named as the vtable
// export names them: slot N is vfunctionN+1.
class tState_MetaLogon : public tGameState, public tStatusAccess
{
public:
	tState_MetaLogon();
	virtual ~tState_MetaLogon();

	// 0x0046d4d2: returns true.
	virtual bool vfunction2();
	// 0x0046e178: ends by calling tGameState::fn_00469e0c (declared void like the base slot;
	// the same code is generated whether or not the result is returned).
	virtual void vfunction3();
	// 0x0046d9cf: returns true.
	virtual bool vfunction4();
	// 0x0046d2a4: returns the state's name, the string at 0x008471e4 ("MetaLogOn").
	virtual char *vfunction13();

	// The tStatusAccess slot, 0x0046d370: calls 0x0046d2aa on this state with the text's pointer.
	virtual void vfunction1( const std::string& text );

	// The constructor leaves +0x20 to +0x24, +0x110 to +0x120 and +0x18c alone; 0x0046d4d2
	// clears the flags.
	std::vector< std::string > field_10;		// the destructor (0x00402d42) destroys 0x10-byte elements
	unsigned char field_20;
	unsigned char field_21;
	unsigned char field_22;
	unsigned char field_23;
	unsigned char field_24;
	tString field_28;
	tValueI< tString > field_50;
	tValueI< tString > field_90;
	tValueI< tString > field_d0;
	unsigned char field_110;
	unsigned char field_111;
	int field_114;
	unsigned char field_118;
	int field_11c;
	char field_120[4];
	// HYPOTHESIS: an object of 0x64 bytes at most: constructed by 0x0045e574 and destroyed by
	// 0x004d2636 (fn_004d2636_object in unsorted/fn_004D2000.cpp), which is declared in that unit
	// only. It is not placed here until that class has a header.
	char field_124[0x64];
	unsigned char field_188;
	// HYPOTHESIS: a pointer: 0x0046e178 loads it into ecx for a call.
	void *field_18c;
	// The destructor (0x00571a7a) is the one shared by every vector of pointers.
	fn_005717f7_vector field_190;
};

typedef char tState_MetaLogon_size_check[ sizeof( tState_MetaLogon ) == 0x1a0 ? 1 : -1 ];

#endif	// tState_MetaLogon_h
