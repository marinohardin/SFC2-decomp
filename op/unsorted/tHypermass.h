//	tHypermass.h: tHypermass<N>, a square grid of cells the hypermass effect draws.
// Evidence:  class (RTTI; original file unknown)

#ifndef tHypermass_h
#define tHypermass_h 1

#include "Tiger/tvectormath.h"

// Seven instances exist, static objects with N = 12, 16, 20, 24, 28, 32 and 36 (constructors
// 0x004f857c, 0x004f8a02, 0x004f8e88, 0x004f930e, 0x004f9794, 0x004f9c1a, 0x004fa0a0; the
// vtables 0x007aa5c4 to 0x007aa624 each hold one slot, the draw routine). The constructors are
// identical but for the constants and call the shared builder 0x004fa6ec, which indexes the
// cells as row * N + column with 0x10 bytes per cell; consecutive instances in the data
// segment are 0x1c + 0x10 * N * N bytes apart (rounded up to 4 or 8). The first field holds N.
template< int N >
class tHypermass
{
public:
	tHypermass();
	virtual void vfunction1(int *param_1);
	tPoint3 fn_004fa526(unsigned int param_2, unsigned int param_3);
	tPoint2 fn_004fa58f(unsigned int param_2, unsigned int param_3);
	void fn_004fa6ec();

	int field_04;	// N
	float field_08;	// 1 / 12 in the first instance
	float field_0c;
	float field_10;
	float field_14;
	float field_18;
	// One cell: three floats and a fourth that the builder fills with a falloff weight.
	float field_1c[N * N][4];
};

typedef char tHypermass_12_size_is_0x91c[ sizeof( tHypermass< 12 > ) == 0x91c ? 1 : -1 ];
typedef char tHypermass_16_size_is_0x101c[ sizeof( tHypermass< 16 > ) == 0x101c ? 1 : -1 ];
typedef char tHypermass_20_size_is_0x191c[ sizeof( tHypermass< 20 > ) == 0x191c ? 1 : -1 ];
typedef char tHypermass_24_size_is_0x241c[ sizeof( tHypermass< 24 > ) == 0x241c ? 1 : -1 ];
typedef char tHypermass_28_size_is_0x311c[ sizeof( tHypermass< 28 > ) == 0x311c ? 1 : -1 ];
typedef char tHypermass_32_size_is_0x401c[ sizeof( tHypermass< 32 > ) == 0x401c ? 1 : -1 ];
typedef char tHypermass_36_size_is_0x511c[ sizeof( tHypermass< 36 > ) == 0x511c ? 1 : -1 ];

#endif	// tHypermass_h
