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

	unsigned char field_00[ 8 ];
};

#endif	// tCompassRose_h
