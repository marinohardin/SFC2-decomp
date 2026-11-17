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

	unsigned char field_04[ 4 ];
};

#endif	// tCompassRose_h
