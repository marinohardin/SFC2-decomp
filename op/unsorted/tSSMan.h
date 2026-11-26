//	tSSMan.h: tSSMan (RTTI, vtable 0x007a1778), the base of tTacticalSubStateMan.
// Evidence:  class (RTTI; original file unknown)

#ifndef tSSMan_h
#define tSSMan_h 1

// HYPOTHESIS: the vtable has five slots: the destructor (0x004bc959 is the scalar deleting
// destructor) and four more, all shared stubs (0x0040c74d, then 0x00737f86 three times); their
// types are not known. The class holds nothing but the vtable pointer (4 bytes).
class tSSMan
{
public:
	tSSMan();
	virtual ~tSSMan();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
};

typedef char tSSMan_size_check[ sizeof( tSSMan ) == 0x4 ? 1 : -1 ];

#endif
