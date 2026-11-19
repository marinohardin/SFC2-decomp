//	tButtonAsset.h: tButtonAsset and the button assets derived from it.
// Evidence:  class (RTTI: tButtonAsset type descriptor 0x00882ff8, whose bases are tControlAsset
// and tAsset; tPushBtnAsset, tPushRectAsset, tTextButtonAsset and tValueBtnAsset derive from
// it; original file unknown)

#ifndef tButtonAsset_h
#define tButtonAsset_h 1

#include "unsorted/tAsset.h"
#include "unsorted/tSpriteCntl.h"

// The layouts come from the asset factory fn_00676a30; the sizes are the ones it allocates.
class tButtonAsset : public tControlAsset
{
public:
	tButtonAsset();
	virtual ~tButtonAsset();

	// HYPOTHESIS: field names and types unknown.
	unsigned char field_B0[ 0x118 - 0xb0 ];
};

class tPushBtnAsset : public tButtonAsset
{
public:
	tPushBtnAsset();
	virtual ~tPushBtnAsset();

	tSpriteCntl field_118;
};

inline tPushBtnAsset::tPushBtnAsset()
{
}

class tPushRectAsset : public tButtonAsset
{
public:
	tPushRectAsset();
	virtual ~tPushRectAsset();

	int field_118;
};

inline tPushRectAsset::tPushRectAsset()
{
}

class tTextButtonAsset : public tButtonAsset
{
public:
	tTextButtonAsset();
	virtual ~tTextButtonAsset();

	c_00684c00 field_118;
};

inline tTextButtonAsset::tTextButtonAsset()
{
}

class tValueBtnAsset : public tButtonAsset
{
public:
	tValueBtnAsset();
	virtual ~tValueBtnAsset();

	short field_118;
	tSpriteCntl field_11C;
};

inline tValueBtnAsset::tValueBtnAsset()
{
	field_118 = 1;
}

#endif	// tButtonAsset_h
