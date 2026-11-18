//	fn_0090bed4_Object.h: the object the pointer at 0x0090bed4 names.
// Evidence:  calls from tSfcApp3d and the fleet entry (original name and file unknown)

#ifndef fn_0090bed4_Object_h
#define fn_0090bed4_Object_h 1

class tObject_005235a7;
class tCommandDestination;

// HYPOTHESIS: updated through 0x00522681.
class fn_0090bed4_Object {
public:
	void fn_00522681();
	// HYPOTHESIS: forgets an entity; tEntity's destructor (0x0040ffc9) calls it when the
	// entity's +0x64 is set.
	void fn_005229b9(class tEntity *pEntity);
	tObject_005235a7 *fn_005235a7(int param_1);
	// HYPOTHESIS: hands the fleet with this ID a new command destination, as 0x004c3815 calls it.
	void fn_00522bdf(int param_1, tCommandDestination *pDestination);
	// HYPOTHESIS: takes a float taken from the table of doubles at 0x008552d0.
	void fn_00522c86(float param_1);
};

extern fn_0090bed4_Object *g_p0090bed4;

#endif // fn_0090bed4_Object_h
