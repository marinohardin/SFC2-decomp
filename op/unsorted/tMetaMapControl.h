#ifndef TMETAMAPCONTROL_H
#define TMETAMAPCONTROL_H

#include <vector>

#include "unsorted/tValueBtn.h"

class tCompassRose;

// HYPOTHESIS: 0x64-byte records 0x005e32d7 gets in a vector from 0x005e6aeb and hands one by one
// to 0x005e381f; the layout is unknown.
struct fn_005e381f_entry
{
	unsigned char field_0[ 0x64 ];
};

// HYPOTHESIS: 0x64-byte records 0x005e32d7 gets in a vector from 0x005e6dea; flag word at +0x4c,
// the value 0x005e37c4 takes at +0x50.
struct fn_005e36cd_entry
{
	unsigned char field_0[ 0x4c ];
	int field_4c;
	int field_50;
	unsigned char field_54[ 0x64 - 0x54 ];
};

// HYPOTHESIS: the object at +0x17c; 0x005e3148 calls 0x005eadc8 on it when it is set.
class fn_005eadc8_class
{
public:
	void fn_005eadc8();
	void fn_005eadcd();
	void fn_005eae34();
	void fn_005eae4a();
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
	void fn_005e36a3();
	void fn_005e36cd( fn_005e36cd_entry *entry, int param_2 );
	void fn_005e37c4( fn_005e36cd_entry *entry, int param_2, int param_3 );
	void fn_005e381f( fn_005e381f_entry *entry, int param_2 );
	void fn_005e3b86();

	fn_005eadc8_class *field_17c;
	tCompassRose *field_180;
	// HYPOTHESIS: four vectors (an allocator byte and three pointers each); element types unknown.
	std::vector<int> field_184;
	std::vector<int> field_194;
	std::vector<int> field_1a4;
	std::vector<int> field_1b4;
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
