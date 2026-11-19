//	tObject_005d27f1.h
//
//	The object fn_005d27f1 returns. Only what the rebuilt functions need is declared yet.
//
#if !defined( tObject_005d27f1_h )
#define tObject_005d27f1_h

#pragma once

#include "Common/SmartPtr.h"
#include "unsorted/nSwitch__tAddress.h"

// HYPOTHESIS: fn_005d3679 (which reads the text "*~Server~*") and fn_005d2f20 return, held by a
// smart pointer, reference-counted objects the switch sends to (see unsorted/nSwitch__tAddress.h).
// fn_005d27f1 returns the object they are called on. Its class and name are not known.
class tObject_005d27f1
{
public:
	tSmartPtr<nSwitch::tAddress> fn_005d2f20();
	tSmartPtr<nSwitch::tAddress> fn_005d3679();
};
tObject_005d27f1* fn_005d27f1();

#endif
