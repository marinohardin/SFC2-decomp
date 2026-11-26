//	tStatusAccess.h: tStatusAccess, the interface a state implements to take a status text.
// Evidence:  class (RTTI; vtable 0x0079f2d4)

#ifndef tStatusAccess_h
#define tStatusAccess_h 1

#include <string>

// The vtable at 0x0079f2d4 has one slot, the purecall stub 0x00737f86, and the class is 4 bytes.
// tState_MetaLogon implements slot 0 at 0x0046d370, which takes one argument by reference and
// reads a pointer at +4 of it, falling back to the empty string at 0x0079564c when that is null:
// the shape of std::string::c_str() under VC6.
class tStatusAccess
{
public:
	virtual void vfunction1( const std::string& text ) = 0;
};

#endif	// tStatusAccess_h
