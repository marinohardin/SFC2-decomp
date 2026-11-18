#ifndef UNSORTED_TOBJECT_00918888_H
#define UNSORTED_TOBJECT_00918888_H

// HYPOTHESIS: the object at 0x00918888; only slot 8 (offset 0x20) is called, returning an int
// (tSndMgr_Miles.cpp reads it as a game state).
class tObject_00918888
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual int vfunction8();
};

extern tObject_00918888 *gObject_00918888;

#endif
