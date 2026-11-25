#include "Common/BaseTypes.h"

// HYPOTHESIS: this is the tAIUnitData constructor (no header declares the class yet);
// only the members this function writes are declared.
class tAIUnitData
{
public:
	int field_0;
	int field_4;
	float field_8;
	float field_c;
	float field_10;
	float field_14;
	float field_18;
	int field_1c;
	float field_20;
	float field_24;
	int field_28;
	float field_2c;
	float field_30;
	char field_34;
	int field_38;
	int field_3c;

	tAIUnitData();
	tAIUnitData(int a, int b, int c, float d, float e, char f);
};

// HYPOTHESIS: the two scale factors are constants at fixed exe addresses.
extern float g_007b01e0;
extern float g_007b01e4;

// FUNCTION: STARFLEETOP 0x00520ab2
tAIUnitData::tAIUnitData(int a, int b, int c, float d, float e, char f)
{
	field_c = d;
	field_10 = d;
	field_14 = d;
	field_0 = a;
	field_4 = b;
	field_1c = 0;
	field_28 = 0;
	field_3c = -1;
	field_8 = e;
	field_20 = 0.0f;
	field_34 = f;
	field_24 = 0.0f;
	field_38 = c;
	field_18 = d * g_007b01e0;
	field_2c = d * g_007b01e4;
	field_30 = e / d;
}

// FUNCTION: STARFLEETOP 0x00520b1a
// HYPOTHESIS: the default constructor of tAIUnitData (same -1 sentinels as the other one).
tAIUnitData::tAIUnitData()
{
	field_0 = -1;
	field_4 = -1;
	field_c = 0;
	field_3c = -1;
	field_10 = 0;
	field_1c = 0;
	field_28 = 0;
	field_14 = 0;
	field_34 = 0;
	field_38 = 0;
	field_18 = 0;
	field_20 = 0;
	field_24 = 0;
	field_2c = 0;
	field_30 = 0;
}

// HYPOTHESIS: owner of the 0x32-entry tAIUnitData array at +8 (count at +0xc88); real class unknown.
class tAIUnitDataList
{
public:
	char pad_0[8];
	// HYPOTHESIS: the array and its count form one member at +8 (the count is addressed from the array base).
	struct tList
	{
		tAIUnitData field_0[0x32];
		int field_c80;

		// MATCH-ONLY: inlined append helper; the original's name is unknown.
		void Append(const tAIUnitData& d)
		{
			if (field_c80 < 0x32) {
				field_0[field_c80] = d;
				field_c80++;
			}
		}
	} field_8;

	int fn_00520cba(int a, int b, float c, char d);
	tAIUnitData* fn_00520d38(int a);
};

// FUNCTION: STARFLEETOP 0x00520cba
int tAIUnitDataList::fn_00520cba(int a, int b, float c, char d)
{
	if (field_8.field_c80 < 0x32) {
		tAIUnitData data(field_8.field_c80, a, b, c, c, d);
		tAIUnitData* p = fn_00520d38(a);
		if (p == 0) {
			field_8.Append(data);
		} else {
			*p = data;
		}
		return data.field_0;
	}
	return -1;
}
