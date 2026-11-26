//	tState_Multiplayer.h: tState_Namespace_Multiplayer::tState_Multiplayer, the multiplayer state.
// Evidence:  class (RTTI; vtable 0x0079f32c), the constructor 0x004737c7 and the destructor 0x00473a04

#ifndef tState_Multiplayer_h
#define tState_Multiplayer_h 1

#include "Tiger/tstring.h"
#include "unsorted/tValueI.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tGameState.h"
#include "unsorted/tShipUI.h"
#include "unsorted/fn_005717f7_vector.h"

namespace tState_Namespace_Multiplayer
{

// Derives from tGameState alone. The single global instance is at 0x0090e438 (constructed by
// 0x004737bd, destroyed by 0x00473c2a); the next global in the image starts at 0x00910648, which
// gives the size, 0x2210, and the last offset the methods use is +0x220c. Members are placed by
// what the constructor (0x004737c7) and destructor (0x00473a04) build and by the offsets the
// methods use; the gaps the original never touches in those two are char arrays.
//
// The vtable at 0x0079f32c overrides slot 0 (0x004739e8) and slots 1, 2, 3, 5, 12 and 14; every
// other slot keeps tGameState's. The virtual functions are named as the vtable export names them
// (slot N is vfunctionN+1).
class tState_Multiplayer : public tGameState
{
public:
	tState_Multiplayer();
	virtual ~tState_Multiplayer();

	// 0x0046ed6a (slot 1)
	virtual bool vfunction2();
	// 0x0046f292, 6,108 bytes (slot 2)
	virtual void vfunction3();
	// 0x0046f197 (slot 3)
	virtual bool vfunction4();
	// 0x004714b7 (slot 5)
	virtual void vfunction6(int param_1);
	// 0x0046ed64: six bytes, so it returns a constant (the name slot of the other states).
	virtual char *vfunction13();
	// 0x0047156b (slot 14)
	virtual void vfunction15(c_0046aa10_message *param_1);

	// HYPOTHESIS: 0x0046f24b is a method; it reads the flags at +0x2200.
	int fn_0046f24b( int param_1 );
	void fn_00470b59( unsigned char param_1 );
	void fn_00470c38( unsigned char param_1 );
	// HYPOTHESIS: 0x00473049 is a method taking a pointer to a list entry of field_e64.
	char fn_00473049( void *entry );
	// HYPOTHESIS: 0x00472e88 makes an object (stored through the first argument) and returns its index.
	int fn_00472e88( char **param_1, int param_2 );
	void fn_00472356();
	void fn_00470d4c();

	int field_0c;
	unsigned char field_10;
	unsigned char field_11;
	unsigned char field_12;
	char fPad_13;
	// HYPOTHESIS: 0x84 bytes, built by 0x004309ad (the constructor leaves it to that call); the
	// methods read bytes at +0x1d (tested against 1 and 2) and pointers inside it.
	char field_14[0x84];
	// Six at a time, built by the vector constructor 0x00736367 with 0x0043d2ef and destroyed with
	// 0x0043ac79 (tValueI<tString>'s).
	tValueI< tString > field_98[6];
	tValueI< tString > field_218[6];
	// HYPOTHESIS: 0x18 bytes the constructor leaves alone; 0x0047134d takes the address of +0x398.
	char field_398[0x18];
	tValueI< tString > field_3b0[6];
	// tValue<long> (0x00473c3a builds each, 0x004a719c destroys).
	tValue< long > field_530[6];
	tValueI< tString > field_5d8[6];
	tValueI< tString > field_758[6];
	char field_8d8[0x20];
	tValueI< tString > field_8f8;
	tValueI< tString > field_938;
	tValueI< tString > field_978[6];
	tValueI< tString > field_af8[6];
	tValueI< tString > field_c78;
	tValueI< tString > field_cb8;
	tValueI< tString > field_cf8;
	tValueI< tString > field_d38;
	char field_d78[0x18];
	// A state number: 0x00470b74 and 0x00470c53 test it and set it to 0 to 3.
	int field_d90;
	char field_d94[0x90];
	tValueI< tString > field_e24;
	// A tShipUI (0x004cd034, destroyed by 0x004cd115); its size is where the next member starts.
	tShipUI field_e64;
	// The destructor (0x00571a7a) is the one shared by every vector of pointers.
	fn_005717f7_vector field_1f68;
	// +0x1f7c to +0x1f8c are ints the methods compare and store; +0x1f90, +0x1f98 and +0x1fa0 are
	// read and written too (width not known).
	char field_1f78[0x2c];
	// Six elements of 0x5c bytes, built with 0x0047292b and destroyed with 0x0047296f.
	char field_1fa4[6 * 0x5c];
	int field_21cc;
	int field_21d0;
	tString field_21d4;
	unsigned char field_21fc;
	// HYPOTHESIS: flags; 0x00471fb4 tests bit 2.
	int field_2200;
	int field_2204;
	int field_2208;
	int field_220c;
};

typedef char tState_Multiplayer_size_check[ sizeof( tState_Multiplayer ) == 0x2210 ? 1 : -1 ];

}	// namespace tState_Namespace_Multiplayer

#endif	// tState_Multiplayer_h
