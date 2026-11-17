//	tValueBase.h: tNotifier and tValueBase, the bases of the value objects (tValue<T>).
// Evidence:  class (RTTI; original file unknown)

#ifndef tValueBase_h
#define tValueBase_h 1

// HYPOTHESIS: 0x18 bytes (the class recovery's estimate); only its destructor (0x006711c0) is
// known.
class tNotifier
{
public:
	virtual ~tNotifier();

	// HYPOTHESIS: called with (3, 0) right after a value object's value is set; a notify step.
	void fn_006712c0( long arg1, long arg2 );

	char m_04[0x14];
};

// The destructor is inline: the exe's copy at 0x004a719c only stores the vtable (0x00799570)
// and jumps to tNotifier's, and the derived classes' destructors repeat those two steps.
class tValueBase : public tNotifier
{
public:
	tValueBase();
	virtual ~tValueBase() {}
};

#endif	// tValueBase_h
