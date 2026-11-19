//	fn_005e5bd7_class.h
//
//	HYPOTHESIS: the relay the map initialize procedure is built with (validate( fpMyRelay ));
//	its methods sit in the original's MetaViewPortHandler.cpp. unsorted/tMetaMapControl.cpp
//	declares what may be the same object locally as fn_005e5e24_class. Of its vtable only the
//	slot 8 virtual is known. Its class name is not recovered.
//
#if !defined( fn_005e5bd7_class_h )
#define fn_005e5bd7_class_h

#include "Common/BaseTypes.h"

class fn_005e5bd7_class
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual int32 vfunction8( int32 param_1 );

	void fn_005e5bd7( int32 param_1 );
	void fn_005e5bfe( int32 param_1 );
	void fn_005e5ef4();
};

#endif
