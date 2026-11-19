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

class tBoundAsset : public tAsset
{
public:
	virtual ~tBoundAsset();
};

class tStaticTextAsset : public tBoundAsset
{
public:
	virtual ~tStaticTextAsset();

	short field_5C;	// HYPOTHESIS: the text's width
	short field_5E;	// HYPOTHESIS: the text's height
	tString field_60;	// HYPOTHESIS: the text
	unsigned int field_88;	// HYPOTHESIS: the font: size in the low byte, style in the high byte
	unsigned int field_8C;	// HYPOTHESIS: a colour
	unsigned int field_90;	// HYPOTHESIS: a second colour
};

// RTTI: tControlAsset's bases are tAsset; tSliderAsset's are tControlAsset and tAsset
// (type descriptor 0x00853b50).
class tControlAsset : public tAsset
{
public:
	virtual ~tControlAsset();
};

class tSliderAsset : public tControlAsset
{
public:
	virtual ~tSliderAsset();

	// HYPOTHESIS: field types and names unknown; tScrollBar::mLoad (0x004c878a) reads these.
	unsigned char field_5C[ 0xb0 - 0x5c ];
	short field_B0;	// HYPOTHESIS: the bar's width
	short field_B2;	// HYPOTHESIS: the bar's height
	int field_B4;	// HYPOTHESIS: identifies the thumb's image
	tString field_B8;	// HYPOTHESIS: a name; the part before the first '.' is kept
};

#endif	// tAsset_h
