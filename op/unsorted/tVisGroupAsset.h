//	tVisGroupAsset.h: tVisGroupAsset and tRadioGroupAsset.
// Evidence:  class (RTTI: tVisGroupAsset type descriptor 0x00853990, tRadioGroupAsset
// 0x00882fb8, whose bases are tVisGroupAsset and tAsset; original file unknown)

#ifndef tVisGroupAsset_h
#define tVisGroupAsset_h 1

#include "unsorted/tAsset.h"
#include <vector>

// The layouts come from the asset factory fn_00676a30; the sizes are the ones it allocates.
class tVisGroupAsset : public tAsset
{
public:
	tVisGroupAsset();
	virtual ~tVisGroupAsset();

	std::vector<void *> field_5C;	// HYPOTHESIS: the element type is not known
	c_00678500 field_6C;
	int field_BC;
};

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
