//	tTacticalSubStateMan.h: tTacticalSubStateMan (RTTI, vtable 0x007a175c, 8 bytes), a tSSMan.
// Evidence:  class (RTTI; original file unknown)

#ifndef tTacticalSubStateMan_h
#define tTacticalSubStateMan_h 1

#include "unsorted/tSSMan.h"

// The constructor (0x004bc911) clears the word after the vtable pointer and stores the vtable.
// HYPOTHESIS: the vtable has six slots: the destructor (0x004bc936 is its scalar deleting
// destructor), then 0x0040c74d (tSSMan's), 0x00407d08, 0x004088da, 0x004bc976 and 0x004bc91e.
// 0x004bc976 takes one argument (a name) and returns the object of the list at 0x0091a1e0 whose
// name matches; 0x004bc91e calls it and then the slot after it. Types are not known.
class tTacticalSubStateMan : public tSSMan
{
public:
	tTacticalSubStateMan();
	virtual ~tTacticalSubStateMan();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();

	// HYPOTHESIS: a pointer (null from the constructor).
	void *m_04;
};

typedef char tTacticalSubStateMan_size_check[ sizeof( tTacticalSubStateMan ) == 0x8 ? 1 : -1 ];

#endif
