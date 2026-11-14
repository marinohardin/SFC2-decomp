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

	char m_04[0x14];
};

// The destructor is inline: the exe's copy at 0x004a719c only stores the vtable (0x00799570)
// and jumps to tNotifier's, and the derived classes' destructors repeat those two steps.
class tValueBase : public tNotifier
{
public:
	virtual ~tValueBase() {}
};

#endif	// tValueBase_h
