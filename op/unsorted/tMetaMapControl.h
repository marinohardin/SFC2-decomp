#ifndef TMETAMAPCONTROL_H
#define TMETAMAPCONTROL_H

#include <vector>

#include "unsorted/tValueBtn.h"

class tCompassRose;

// HYPOTHESIS: 0x64-byte records 0x005e32d7 gets in a vector from 0x005e6dea; flag word at +0x4c,
// the value 0x005e37c4 takes at +0x50; 0x005e3511 compares +0x14 with 5.
struct fn_005e36cd_entry
{
	// HYPOTHESIS: 0x005e3935 calls vtable slot 0 on an entry with the control's +0x1c8 and uses
	// the answer as an index.
	virtual int vfunction0( int param_1 );

	unsigned char field_4[ 0x14 - 0x4 ];
	int field_14;
	unsigned char field_18[ 0x2c - 0x18 ];
	// HYPOTHESIS: the point 0x005e374b draws at when it is given none.
	short field_2c;
	short field_2e;
	unsigned char field_30[ 0x40 - 0x30 ];
	bool field_40;
	unsigned char field_41[ 0x44 - 0x41 ];
	// HYPOTHESIS: 0x005e3a37 scales the colour when this is 2.
	int field_44;
	unsigned char field_48[ 0x4c - 0x48 ];
	int field_4c;
	int field_50;
	unsigned char field_54[ 0x64 - 0x54 ];
};

// HYPOTHESIS: 0x64-byte records 0x005e32d7 gets in a vector from 0x005e6aeb and hands one by one
// to 0x005e381f, which passes them to 0x005e374b and reads +0x14 and +0x2c like the records
// above; so the same layout is assumed. 0x005e381f calls 0x005e488e on one for a point.
struct fn_005e381f_entry : public fn_005e36cd_entry
{
	const tPoint &fn_005e488e();
};

// HYPOTHESIS: the object at +0x17c; 0x005e3148 calls 0x005eadc8 on it when it is set.
class fn_005eadc8_class
{
public:
	// HYPOTHESIS: 0x005e36a3 deletes it through vtable slot 0 with 1, a virtual destructor.
	virtual ~fn_005eadc8_class();
	void fn_005eadc8();
	void fn_005eadcd();
	void fn_005eae34();
	void fn_005eae4a();
	// HYPOTHESIS: 0x005e361d draws the image 0x005eaddc returns at the point (two shorts)
	// 0x005eae6e returns, and calls 0x005e36a3 when 0x005eae11 answers false.
	c_tScrollBar_FieldA8 *fn_005eaddc();
	const tPoint &fn_005eae6e();
	bool fn_005eae11();
};

// HYPOTHESIS: 0x1ec bytes, from the operator new in the factory at 0x005e2f60.
// vftables 0x007b50b0 (tButton part) and 0x007b50a4 (tVisNotifiee part).
class tMetaMapControl : public tValueBtn
{
public:
	tMetaMapControl();
	virtual ~tMetaMapControl();
	// HYPOTHESIS: the tVisNotifiee slot 0x005e469e overrides; the exe's vftable 0x007b50a4 has two
	// slots, so it is the one after the first.
	virtual void vfunction1();

	void fn_005e3148( int param_1 );
	void fn_005e31be();
	void fn_005e31dd( int param_1 );
	void fn_005e326c( int param_1 );
	void fn_005e32d7( int param_1 );
	void fn_005e33f4( int param_1 );
	void fn_005e3511( int param_1 );
	void fn_005e361d( const tRect *area );
	void fn_005e36a3();
	void fn_005e36cd( fn_005e36cd_entry *entry, int param_2 );
	void fn_005e370c( fn_005e36cd_entry *entry, int param_2 );
	// HYPOTHESIS: 0x005e36cd passes the entry, its second argument, the element of the vector
	// at +0x1a4 the entry's +0x14 selects, and three -1. It draws that image at a point (the
	// entry's own when both are -1) in a colour (0x005e3935's when -1), red, green and blue bytes.
	void fn_005e374b( fn_005e36cd_entry *entry, int param_2, c_tScrollBar_FieldA8 *image, int x, int y, unsigned long color );
	unsigned long fn_005e3935( fn_005e36cd_entry *entry );
	unsigned char *fn_005e396d( int param_1 );
	unsigned long fn_005e397b( unsigned char *param_1 );
	void fn_005e3a37( fn_005e36cd_entry *entry, unsigned long *color );
	void fn_005e3ad8();
	void fn_005e3b6e();
	void fn_005e37c4( fn_005e36cd_entry *entry, int param_2, int param_3 );
	void fn_005e37d8( fn_005e36cd_entry *entry, int param_2, int param_3 );
	void fn_005e381f( fn_005e381f_entry *entry, int param_2 );
	void fn_005e3891( fn_005e381f_entry *entry, int param_2 );
	void fn_005e3903( fn_005e381f_entry *entry, int param_2 );
	void fn_005e3b86();
	void fn_005e3ba5();
	void fn_005e3d73();
	void fn_005e3e48();
	void fn_005e3f1d();
	void fn_005e4455();

	fn_005eadc8_class *field_17c;
	tCompassRose *field_180;
	// HYPOTHESIS: four vectors (an allocator byte and three pointers each); element types unknown.
	std::vector<c_tScrollBar_FieldA8 *> field_184;
	std::vector<c_tScrollBar_FieldA8 *> field_194;
	std::vector<c_tScrollBar_FieldA8 *> field_1a4;
	std::vector<c_tScrollBar_FieldA8 *> field_1b4;
	int field_1c4;
	int field_1c8;
	unsigned char field_1cc[ 0x1d4 - 0x1cc ];
	int field_1d4;
	int field_1d8;
	tRect field_1dc;
	int field_1e4;
	bool field_1e8;
};

#endif
