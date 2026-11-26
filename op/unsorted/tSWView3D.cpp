// Unit:      unsorted/tSWView3D.cpp
// Functions: 0x0069f480 to 0x0069fdc0, 25 in op/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   sdk-release (harness/units.json)

#include <string.h>
#include "unsorted/tSWView3D.h"

// HYPOTHESIS: field_1f68 is an object whose slot +0x2c returns an object with a slot +0x48 that
// takes the 16-bit value at field_1f5c and the clipped rectangle.
class fn_0069fb90_Fill {
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual void v3();
	virtual void v4();
	virtual void v5();
	virtual void v6();
	virtual void v7();
	virtual void v8();
	virtual void v9();
	virtual void v10();
	virtual void v11();
	virtual void v12();
	virtual void v13();
	virtual void v14();
	virtual void v15();
	virtual void v16();
	virtual void v17();
	virtual void Fill(unsigned short, int *);
};

class fn_0069fb90_Surface {
public:
	virtual void v0();
	virtual void v1();
	virtual void v2();
	virtual void v3();
	virtual void v4();
	virtual void v5();
	virtual void v6();
	virtual void v7();
	virtual void v8();
	virtual void v9();
	virtual void v10();
	virtual fn_0069fb90_Fill *Get();
};

// FUNCTION: STARFLEETOP 0x0069fb90
bool tSWView3D::vfunction29(int a, int b, int c, int d, bool e, bool f)
{
	int rect[4];
	rect[2] = field_1f4c + c;
	rect[0] = field_1f4c + a;
	rect[1] = field_1f50 + b;
	int bottom = field_1f50 + d;
	if (rect[0] < field_1f4c)
		rect[0] = field_1f4c;
	if (rect[1] < field_1f50)
		rect[1] = field_1f50;
	if (field_1f54 < rect[2])
		rect[2] = field_1f54;
	if (field_1f58 < bottom)
		bottom = field_1f58;
	rect[3] = bottom;
	if (rect[2] <= rect[0] || rect[3] <= rect[1])
		return false;

	if (e && field_1f68) {
		fn_0069fb90_Fill *fill = ((fn_0069fb90_Surface *)field_1f68)->Get();
		fill->Fill(field_1f5c, rect);
	}
	if (f && field_5fb4) {
		int width = field_5fb8;
		unsigned int pitch = field_5fc0;
		unsigned short *p = field_5fb4 + (width * rect[1] + rect[0]);
		int rows = rect[3] - rect[1];
		if (rows < 0)
			rows = 0;
		for (; rows > 0; rows--) {
			memset(p, 0, pitch);
			p = (unsigned short *)((char *)p + pitch);
		}
	}
	return true;
}
