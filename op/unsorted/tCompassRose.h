//	tCompassRose.h
// Evidence:  class (RTTI; original file unknown)

#ifndef tCompassRose_h
#define tCompassRose_h 1

class tMetaMapControl;

// HYPOTHESIS: 8 bytes, from the operator new in tMetaMapControl's constructor (0x005e2f95),
// which hands it the control.
class tCompassRose
{
public:
	// HYPOTHESIS: 0x005e24cc.
	tCompassRose( tMetaMapControl *pControl );
	// HYPOTHESIS: virtual; tMetaMapControl's destructor (0x005e30ac) deletes it through slot 0
	// with a delete flag of 1.
	virtual ~tCompassRose();
	virtual long vfunction1();
	virtual long vfunction2();

	// tMetaMapControl calls it on its compass rose (0x005e3148).
	void fn_005e2501();
	void fn_005e2539();

	unsigned char field_04[ 4 ];
};

#endif	// tCompassRose_h
