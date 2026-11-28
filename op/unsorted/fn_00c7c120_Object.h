#ifndef UNSORTED_FN_00C7C120_OBJECT_H
#define UNSORTED_FN_00C7C120_OBJECT_H

// The frame timer (class and members from the SDK's symbols): the object at 0x00c7c120 is updated
// first each frame through mNewFrame (0x006a0bb0); mGetTick (0x006a0d50) returns a time.
class tFrameTimer {
public:
	void mNewFrame();
	double mGetTick();
};

extern tFrameTimer g_00c7c120;

#endif
