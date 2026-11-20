//	tSpriteCntl.h: tSpriteCntl.
// Evidence:  class (RTTI: tSpriteCntl type descriptor 0x00853db0, vtable 0x007a3084; original
// file unknown)

#ifndef tSpriteCntl_h
#define tSpriteCntl_h 1

// HYPOTHESIS: 0x00684bb0 is the constructor (the asset factory calls it on a fresh member);
// only the size is known.
class tSpriteCntl
{
public:
	tSpriteCntl();
	virtual ~tSpriteCntl();

	unsigned char field_04[ 0x38 - 0x4 ];
};

#endif	// tSpriteCntl_h
