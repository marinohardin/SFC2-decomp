//	tAsset.h: tAsset, tBoundAsset and tStaticTextAsset, the assets a screen is built from.
// Evidence:  class (RTTI: tAsset type descriptor 0x00853978, tStaticTextAsset 0x008546f0, whose
// bases are tBoundAsset and tAsset; original file unknown)

#ifndef tAsset_h
#define tAsset_h 1

#include "unsorted/tNetControl.h"	// tString

class tStream;

// HYPOTHESIS: only what tListBox_Base::vfunction37 reads is declared; slot names unknown.
class tAsset
{
public:
	tAsset();
	virtual ~tAsset();
	short fn_00675bf0();
	virtual tStream& vfunction5( tStream& s );
	virtual tStream& vfunction6( tStream& s );

	int field_04;
	// HYPOTHESIS: field names and types unknown; the second string sits at an odd 2-byte offset.
#pragma pack(push, 1)
	tString field_08;
	short field_30;
	tString field_32;
	short field_5A;
#pragma pack(pop)
};


// MATCH-ONLY: the original inlines this destructor into the derived ones.
inline tAsset::~tAsset()
{
}

// The layouts below come from the asset factory fn_00676a30, which inlines most constructors;
// the sizes are the ones it allocates.
#pragma pack(push, 1)
class tCastItem : public tAsset
{
public:
	tCastItem();
	virtual ~tCastItem();
	virtual tStream& vfunction5( tStream& s );
	virtual tStream& vfunction6( tStream& s );

	// HYPOTHESIS: field names and types unknown.
	int field_5C;
	short field_60;
	int field_62;
	int field_66;
};

class tBmpAsset : public tCastItem
{
public:
	tBmpAsset();
	virtual ~tBmpAsset();
	virtual tStream& vfunction5( tStream& s );

	// HYPOTHESIS: field names and types unknown.
	unsigned char field_6A;
	unsigned char field_6B;
	unsigned char field_6C;
};
#pragma pack(pop)

inline tCastItem::tCastItem()
{
	field_62 = 0;
	field_5C = 0;
	field_60 = 0;
	field_66 = 0;
}

// MATCH-ONLY: the original expands this constructor in some callers (fn_00676a30 kind 1) and has
// an out-of-line copy for others; tBmpAsset.cpp defines that copy itself.
#ifndef tBmpAsset_out_of_line
inline tBmpAsset::tBmpAsset()
{
	field_6A = 0xff;
	field_6C = 0xff;
	field_6B = 0;
}
#endif

class tBoundAsset : public tAsset
{
public:
	tBoundAsset();
	virtual ~tBoundAsset();
	virtual tStream& vfunction5( tStream& s );
	virtual tStream& vfunction6( tStream& s );

	short field_5C;	// HYPOTHESIS: the width
	short field_5E;	// HYPOTHESIS: the height
};

class tStaticTextAsset : public tBoundAsset
{
public:
	tStaticTextAsset();
	virtual ~tStaticTextAsset();

	// HYPOTHESIS: the factory builds 0x3c bytes at +0x60 with one constructor (0x0068a440), so
	// these four are probably one member of a class not declared yet.
	tString field_60;	// HYPOTHESIS: the text
	unsigned int field_88;	// HYPOTHESIS: the font: size in the low byte, style in the high byte
	unsigned int field_8C;	// HYPOTHESIS: a colour
	unsigned int field_90;	// HYPOTHESIS: a second colour
	unsigned char field_94[ 0x9c - 0x94 ];
};

class tGenericRectAsset : public tBoundAsset
{
public:
	tGenericRectAsset();
	virtual ~tGenericRectAsset();

	tString field_60;	// HYPOTHESIS: a name
};

inline tGenericRectAsset::tGenericRectAsset()
{
}

class tBrowserAsset : public tBoundAsset
{
public:
	tBrowserAsset();
	virtual ~tBrowserAsset();

	tString field_60;	// HYPOTHESIS: a name
	int field_88;
};

inline tBrowserAsset::tBrowserAsset()
{
	field_88 = 0;
}

class tGenericBmpAsset : public tBmpAsset
{
public:
	tGenericBmpAsset();
	virtual ~tGenericBmpAsset();

	tString field_70;	// HYPOTHESIS: a name
};

inline tGenericBmpAsset::tGenericBmpAsset()
{
}

// HYPOTHESIS: a 0x50-byte member of tControlAsset and tVisGroupAsset; constructor 0x00678500.
// The factory's unwind states show no destructor.
class c_00678500
{
public:
	c_00678500();

	unsigned char field_00[ 0x50 ];
};

// HYPOTHESIS: a 0x34-byte member of tTextButtonAsset; constructor 0x00684c00.
class c_00684c00
{
public:
	c_00684c00();
	~c_00684c00();

	unsigned char field_00[ 0x34 ];
};

// RTTI: tControlAsset's bases are tAsset; tSliderAsset's are tControlAsset and tAsset
// (type descriptor 0x00853b50).
class tControlAsset : public tAsset
{
public:
	tControlAsset();
	virtual ~tControlAsset();

	int field_5C;
	c_00678500 field_60;
};

inline tControlAsset::tControlAsset()
{
	field_5C = 0;
}

class tSliderAsset : public tControlAsset
{
public:
	tSliderAsset();
	virtual ~tSliderAsset();

	// HYPOTHESIS: field types and names unknown; tScrollBar::mLoad (0x004c878a) reads these.
	short field_B0;	// HYPOTHESIS: the bar's width
	short field_B2;	// HYPOTHESIS: the bar's height
	int field_B4;	// HYPOTHESIS: identifies the thumb's image
	tString field_B8;	// HYPOTHESIS: a name; the part before the first '.' is kept
};

inline tSliderAsset::tSliderAsset()
{
	field_B2 = 0;
	field_B0 = 0;
	field_B4 = 0;
}

class tScrollBoxAsset : public tControlAsset
{
public:
	tScrollBoxAsset();
	virtual ~tScrollBoxAsset();

	short field_B0;
	short field_B2;
	int field_B4;
	int field_B8;
	tString field_BC;	// HYPOTHESIS: a name
};

inline tScrollBoxAsset::tScrollBoxAsset()
{
	field_B2 = 0;
	field_B0 = 0;
}

class tNavigatorAsset : public tControlAsset
{
public:
	tNavigatorAsset();
	virtual ~tNavigatorAsset();

	short field_B0;
	short field_B2;
	tString field_B4;	// HYPOTHESIS: a name
	int field_DC;
	int field_E0;
};

inline tNavigatorAsset::tNavigatorAsset()
{
	field_B2 = 0;
	field_B0 = 0;
	field_E0 = 0;
	field_DC = 0;
}

class tTextEditAsset : public tControlAsset
{
public:
	tTextEditAsset();
	virtual ~tTextEditAsset();

	// HYPOTHESIS: field names and types unknown.
	unsigned char field_B0[ 0x14c - 0xb0 ];
};

class tTableAsset : public tAsset
{
public:
	tTableAsset();
	virtual ~tTableAsset();

	// HYPOTHESIS: field names and types unknown.
	unsigned char field_5C[ 0x7c - 0x5c ];
};

#endif	// tAsset_h
