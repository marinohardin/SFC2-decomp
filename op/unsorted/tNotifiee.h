//	tNotifiee.h: tNotifiee, the base of the objects a tNotifier tells about changes
//	(RTTI, vtable 0x007a0ecc).
// Evidence:  class (RTTI; the shadow classes and tVisNotifiee derive from it; original file unknown)

#ifndef tNotifiee_h
#define tNotifiee_h 1

class tNotifier;

// HYPOTHESIS: the vtable holds two slots, a function that only returns (0x00401038, shared
// with many classes) and the scalar deleting destructor (0x004a039a). The destructor body
// (0x004a049e) stores the vtable and calls 0x004a0384, which asks the tNotifier at +0x4 to
// drop this object (tNotifier 0x00671410) and clears the pointer; the constructors of the
// derived classes clear +0x4 inline.
class tNotifiee
{
public:
	tNotifiee() { field_0x4 = 0; }
	virtual void vfunction0();
	virtual ~tNotifiee();
	// HYPOTHESIS: the member of the destructor body, called by the two destructors.
	void fn_004a0384();

	tNotifier *field_0x4;
};

typedef char tNotifiee_size_is_0x8[ sizeof( tNotifiee ) == 0x8 ? 1 : -1 ];

#endif	// tNotifiee_h
