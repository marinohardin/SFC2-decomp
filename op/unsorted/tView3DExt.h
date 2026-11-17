//	tView3DExt.h: tView3DExt, the extended view (name from RTTI).
// Evidence:  class (original file unknown)

#ifndef tView3DExt_h
#define tView3DExt_h 1

#include "unsorted/tApp3dBase.h"

// HYPOTHESIS: a 64-byte vertex record; only its size is known (the vertex cache allocates
// 0x40 bytes per entry). The empty constructor is inferred from the null test the
// exe makes after operator new, which only a vector new with a constructor emits.
struct fn_006abe00_Vtx
{
	fn_006abe00_Vtx() {}

	char	data[0x40];
};

// HYPOTHESIS: derived from tView3D (both carry the vtable slots the units call); only the
// vertex cache members that mAssertVtxCache uses are placed.
class tView3DExt : public tView3D {
public:
	void	mAssertVtxCache( int count );

	fn_006abe00_Vtx		*mVtxCache;    // +0x04
	int					mVtxCacheSize;    // +0x08
	fn_006abe00_Vtx		*mVtxCacheEnd;    // +0x0c HYPOTHESIS: the first of the 16 spare entries
};

#endif
