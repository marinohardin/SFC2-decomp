#ifndef TMETAMAPCONTROL_H
#define TMETAMAPCONTROL_H

#include <iosfwd>
#include <vector>

#include "unsorted/tValueBtn.h"
#include "unsorted/tMetaMapHex.h"
#include "unsorted/fn_005717f7_vector.h"
#include "Common/Database/DatabaseID.h"

class tCompassRose;
class fn_005eb3c5_class;
struct c_tListBox_Base_Event;

// HYPOTHESIS: the first 0x50 bytes the two 0x64-byte record types below share: 0x005e374b
// draws either kind, reading +0x14, +0x2c, +0x40 and +0x44 and calling vtable slot 0.
struct fn_005e374b_entry
{
	// HYPOTHESIS: 0x005e3935 calls vtable slot 0 on an entry with the control's +0x1c8 and uses
	// the answer as an index.
	virtual int vfunction0( int param_1 );

	nDatabase::tID field_4;
	int field_14;
	tHexPoint field_18;
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
};

// HYPOTHESIS: the records 0x005e32d7 gets in a vector from 0x005e6aeb and hands one by one to
// 0x005e381f, which calls 0x005e488e on one for a point. 0x005e45a0, the vector's destructor,
// calls 0x005e4638 on each record; that code stores no vtable and destroys the container of
// 0x0053a6cc at +0x50, the tHexPoint at +0x18 and the tID at +0x4, so the original declares no
// destructor: 0x005e4638 is the implicit one the compiler generates, and is left unclaimed.
struct fn_005e36cd_entry : public fn_005e374b_entry
{
	const tPoint &fn_005e488e();

	fn_0053a6cc_map field_50;
};

// HYPOTHESIS: the records 0x005e32d7 gets in a vector from 0x005e6dea; flag word at +0x4c, the
// value 0x005e37c4 takes at +0x50; 0x005e3511 compares +0x14 with 5. 0x005e45d3, the vector's
// destructor, calls 0x005e0d50 on each record; that code stores no vtable and destroys a tID at
// +0x54, the tHexPoint at +0x18 and the tID at +0x4, so it is the implicit destructor too.
struct fn_005e381f_entry : public fn_005e374b_entry
{
	// HYPOTHESIS: 0x005e32d7 and its siblings pass this on as the colour.
	int field_50;
	nDatabase::tID field_54;
};

// MATCH-ONLY: the two record vectors' destructors (0x005e45a0, 0x005e45d3) are written out as
// explicit specializations in unsorted/tMetaMapControl.cpp; declared here so that every unit
// that destroys one calls them out of line.
template<> std::vector<fn_005e36cd_entry>::~vector();
template<> std::vector<fn_005e381f_entry>::~vector();

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
	// HYPOTHESIS: the RTTI class tMovementArrow (constructor 0x005eac36, 0x6c bytes); 0x005e4089
	// makes one from the hex pair it fills.
	fn_005eadc8_class( const fn_005eb3c5_class &pair );

	unsigned char field_4[ 0x6c - 0x4 ];
};

struct c_tScrollBar_FieldA8;
// MATCH-ONLY: the library vector of image pointers that the members below stand for; its
// assign (with the erase 0x005e4606), size and index operator are the ones the control calls.
inline std::vector<c_tScrollBar_FieldA8 *> &fn_005e30ac_images( fn_005717f7_vector &v )
{
	return *(std::vector<c_tScrollBar_FieldA8 *> *)&v;
}

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
	void fn_005e36cd( fn_005e381f_entry *entry, int param_2 );
	void fn_005e370c( fn_005e381f_entry *entry, int param_2 );
	// HYPOTHESIS: 0x005e36cd passes the entry, its second argument, the element of the vector
	// at +0x1a4 the entry's +0x14 selects, and three -1. It draws that image at a point (the
	// entry's own when both are -1) in a colour (0x005e3935's when -1), red, green and blue bytes.
	void fn_005e374b( fn_005e374b_entry *entry, int param_2, c_tScrollBar_FieldA8 *image, int x, int y, unsigned long color );
	unsigned long fn_005e3935( fn_005e374b_entry *entry );
	unsigned char *fn_005e396d( int param_1 );
	unsigned long fn_005e397b( unsigned char *param_1 );
	void fn_005e3a37( fn_005e374b_entry *entry, unsigned long *color );
	void fn_005e3ad8();
	void fn_005e3b6e();
	void fn_005e37c4( fn_005e381f_entry *entry, int param_2, int param_3 );
	void fn_005e37d8( fn_005e381f_entry *entry, int param_2, int param_3 );
	void fn_005e381f( fn_005e36cd_entry *entry, int param_2 );
	void fn_005e3891( fn_005e36cd_entry *entry, int param_2 );
	void fn_005e3903( fn_005e36cd_entry *entry, int param_2 );
	void fn_005e3b86();
	void fn_005e3ba5();
	void fn_005e3d73();
	void fn_005e3e48();
	void fn_005e3f1d();
	// HYPOTHESIS: looks up the image named by the string, the int selecting one of several.
	c_tScrollBar_FieldA8 *fn_005e3f82( const std::string &name, int index );
	void fn_005e41ec();
	void fn_005e4211();
	void fn_005e4236();
	void fn_005e425b();
	void fn_005e4280();
	void fn_005e4293();
	void fn_005e42a6();
	void fn_005e4315( int param_1 );
	void fn_005e433c();
	int fn_005e4351();
	void fn_005e4358( long *msg );
	void fn_005e4417( long *msg );
	void fn_005e4426( long *msg );
	void fn_005e4455();
	void fn_005e4468();
	// HYPOTHESIS: draws the map inside the area given; the slot of tObjBase::vfunction8.
	void vfunction8( const tRect *area );
	void vfunction30();
	bool vfunction31( c_tListBox_Base_Event *event );
	void vfunction43( int param_1 );

	fn_005eadc8_class *field_17c;
	tCompassRose *field_180;
	// HYPOTHESIS: four vectors of image pointers (an allocator byte and three pointers each); the
	// destructor destroys them through 0x00571a7a, so they are declared as fn_005717f7_vector.
	fn_005717f7_vector field_184;
	fn_005717f7_vector field_194;
	fn_005717f7_vector field_1a4;
	fn_005717f7_vector field_1b4;
	int field_1c4;
	int field_1c8;
	unsigned char field_1cc[ 0x1d4 - 0x1cc ];
	// HYPOTHESIS: the time of the last update; 0x005e4089 widens it to a double as unsigned.
	unsigned long field_1d4;
	int field_1d8;
	tRect field_1dc;
	int field_1e4;
	bool field_1e8;
};

#endif
