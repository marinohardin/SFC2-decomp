#ifndef TMETAMAPCONTROL_H
#define TMETAMAPCONTROL_H

#include <vector>

#include "unsorted/tValueBtn.h"

class tCompassRose;

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

	int field_17c;
	tCompassRose *field_180;
	// HYPOTHESIS: four vectors (an allocator byte and three pointers each); element types unknown.
	std::vector<int> field_184;
	std::vector<int> field_194;
	std::vector<int> field_1a4;
	std::vector<int> field_1b4;
	int field_1c4;
	unsigned char field_1c8[ 0x1d4 - 0x1c8 ];
	int field_1d4;
	int field_1d8;
	tRect field_1dc;
	int field_1e4;
	bool field_1e8;
};

#endif
