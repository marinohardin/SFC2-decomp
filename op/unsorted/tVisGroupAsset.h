//	tVisGroupAsset.h: tVisGroupAsset and tRadioGroupAsset.
// Evidence:  class (RTTI: tVisGroupAsset type descriptor 0x00853990, tRadioGroupAsset
// 0x00882fb8, whose bases are tVisGroupAsset and tAsset; original file unknown)

#ifndef tVisGroupAsset_h
#define tVisGroupAsset_h 1

#include "unsorted/tAsset.h"
#include <vector>

class tStream;

// HYPOTHESIS: 0x14-byte element of tVisGroupAsset's vector; slot 0 takes a stream.
class c_0067b2b0
{
public:
	virtual void fn_slot0( tStream& s );
	unsigned char field_04[ 0x10 ];
};

// The layouts come from the asset factory fn_00676a30; the sizes are the ones it allocates.
class tVisGroupAsset : public tAsset
{
public:
	tVisGroupAsset();
	virtual ~tVisGroupAsset();
	// vftable 0x007b7c80, 6 slots (tAsset's): dtor 0x0067b5c0, vfunction2 0x0067b590,
	// vfunction3 0x0067b390, vfunction4 0x0067b580, vfunction5 0x0067b2b0, vfunction6 0x0067aef0.
	// HYPOTHESIS: tAsset.h declares only the dtor, vfunction5 and vfunction6 (HEADER REQUEST in
	// the lane notes: vfunction2 to vfunction4 belong between the dtor and vfunction5), so these
	// three are new slots until it does.
	// HYPOTHESIS: takes an id, makes the object 0x0067ea20 finds for it, stores the id at +0x48
	// of that object and calls its slot at +0x90 with this; returns the object.
	virtual void *vfunction2( int id );
	// HYPOTHESIS: assigns field_6C from the caller's member of the same type (the original
	// expands c_00678500's implicit operator=: vector, map, int and tString in order).
	virtual void vfunction3( const c_00678500& x );
	virtual void vfunction4();	// 4 bytes, a bare return
	// HYPOTHESIS: 0x0067aef0 (the symbol table calls it a deleting destructor, wrongly) writes the
	// stream form of the members, field_BC last.
	virtual tStream& vfunction5( tStream& s );
	virtual tStream& vfunction6( tStream& s );

	// HYPOTHESIS: the element is 0x14 bytes with a virtual slot 0 that writes it to a stream
	// (vfunction5 at 0x0067b2b0 calls it for each element); its real class is not known.
	std::vector<c_0067b2b0> field_5C;
	c_00678500 field_6C;
	int field_BC;
};

typedef char tVisGroupAsset_size_is_0xc0[ sizeof( tVisGroupAsset ) == 0xc0 ? 1 : -1 ];

inline tVisGroupAsset::tVisGroupAsset()
{
}

class tRadioGroupAsset : public tVisGroupAsset
{
public:
	tRadioGroupAsset();
	virtual ~tRadioGroupAsset();

	tString field_C0;	// HYPOTHESIS: a name
};

inline tRadioGroupAsset::tRadioGroupAsset()
{
}

#endif	// tVisGroupAsset_h
