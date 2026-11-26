//	tPlayerShadow.h: tPlayerShadow, the abstract base of tActualPlayer (RTTI, vtable 0x007a0e54).
// Evidence:  class (RTTI: tPlayerShadow is a tConnector; original file unknown)

#ifndef tPlayerShadow_h
#define tPlayerShadow_h 1

#include "unsorted/tConnector.h"

// HYPOTHESIS: the vtable has 26 slots: the three of tConnector, then 23 that are all
// _purecall (0x00737f86), so they are pure virtuals here. tPlayerShadow's own constructor is
// not a function of its own: tActualPlayer's constructor (0x0049f4b2) stores this class's
// vtable straight after tConnector's constructor and later replaces it, and its destructor
// (0x004a12a0) clears the global at 0x00917cdc when it points at the object, then calls
// tConnector's. The members are laid out in tActualPlayer. vfunctionN is slot N-1; the
// argument types are placeholders from what tActualPlayer's overrides do (each of those with
// one argument returns with ret 4); an override must repeat the signature exactly.
class tPlayerShadow : public tConnector
{
public:
	virtual ~tPlayerShadow();
	virtual void vfunction4(int aArg) = 0;
	virtual void vfunction5(void *aObject) = 0;
	virtual void vfunction6(void *aObject) = 0;
	virtual void vfunction7(void *aObject) = 0;
	virtual void vfunction8(void *aObject) = 0;
	virtual void vfunction9(void *aObject) = 0;
	virtual void vfunction10() = 0;
	virtual void vfunction11(void *aObject) = 0;
	virtual void vfunction12(void *aObject) = 0;
	virtual void vfunction13(void *aObject) = 0;
	virtual void vfunction14(void *aObject) = 0;
	virtual void vfunction15(void *aObject) = 0;
	virtual void vfunction16(void *aObject) = 0;
	virtual void vfunction17(void *aObject) = 0;
	virtual void vfunction18(void *aObject) = 0;
	virtual void vfunction19(void *aObject) = 0;
	virtual void vfunction20(void *aObject) = 0;
	virtual void vfunction21(void *aObject) = 0;
	virtual void vfunction22(void *aObject) = 0;
	virtual void vfunction23(void *aObject) = 0;
	virtual void vfunction24(void *aObject) = 0;
	virtual void vfunction25(void *aObject) = 0;
	virtual void vfunction26() = 0;
};

#endif	// tPlayerShadow_h
