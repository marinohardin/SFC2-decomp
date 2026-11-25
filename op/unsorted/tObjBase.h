//	tObjBase.h: tObjBase, the base of every visual object.
// Evidence:  class (RTTI: tObjBase type descriptor 0x00846d50, vftable 0x0079ee08, 36 slots)

#ifndef tObjBase_h
#define tObjBase_h 1

// tObjBase is declared in tVisObject.h together with tVisObject, which belongs to other work, so
// this header only includes it (size 0x60, constructor 0x0067cb70 sets vftable 0x0079ee08, +0x4 to
// +0x5c). What this lane's functions still need from it is a HEADER REQUEST in the lane notes:
// virtual vfunction22 to vfunction36 (slots 21 to 35), and the non-virtual members
// fn_0067cd70, fn_0067cee0, fn_0067cf70, fn_0067d310 and fn_0067d4a0.
#include "unsorted/tVisObject.h"

#endif	// tObjBase_h
