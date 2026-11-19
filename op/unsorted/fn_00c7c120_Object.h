#ifndef UNSORTED_FN_00C7C120_OBJECT_H
#define UNSORTED_FN_00C7C120_OBJECT_H

// HYPOTHESIS: an object at 0x00c7c120 updated first each frame through 0x006a0bb0; 0x006a0d50
// returns a time on the FPU stack.
class fn_00c7c120_Object {
public:
	void fn_006a0bb0();
	double fn_006a0d50();
};

extern fn_00c7c120_Object g_00c7c120;

#endif
