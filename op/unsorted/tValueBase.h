//	tValueBase.h: tNotifier and tValueBase, the bases of the value objects (tValue<T>).
// Evidence:  class (RTTI; original file unknown)

#ifndef tValueBase_h
#define tValueBase_h 1

// HYPOTHESIS: 0x18 bytes (the class recovery's estimate); only its destructor (0x006711c0) is
// known.
class tNotifier
{
public:
	// body at 0x004365cd
	tNotifier();
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

// RTTI names tValue<long> (vtable 0x0079955c) and tValue<float> (0x0079bc18); both share
// slot 1 (0x00436a53). The value sits at +0x18.
template <class T>
class tValue : public tValueBase
{
public:
	// HYPOTHESIS: takes one integer; 0x0043e642 passes an int member to it on a tMinMaxFloat.
	virtual void vfunction2( long param_1 );
	// HYPOTHESIS: the set step, as tValueI<tString>'s: assign, then notify with (3, 0).
	tValue &operator=( const T &value ) { m18 = value; fn_006712c0( 3, 0 ); return *this; }

	T m18;
};

// RTTI: tMinMaxFloat : tValue<float> (vtable 0x00799578).
class tMinMaxFloat : public tValue<float>
{
public:
	// body at 0x00436612
	tMinMaxFloat();
	// HYPOTHESIS: slots 2 to 4 each take one float (tBatterySystem's constructor passes
	// 0, 0 and the capacity); probably the minimum, the value and the maximum.
	virtual void vfunction3( float param_1 );
	virtual void vfunction4( float param_1 );
	virtual void vfunction5( float param_1 );
	virtual void vfunction6();
	virtual void vfunction7( float param_1 );

	float m1c;
	float m20;
};

#endif	// tValueBase_h
