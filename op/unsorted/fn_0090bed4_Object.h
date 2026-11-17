//	fn_0090bed4_Object.h: the object the pointer at 0x0090bed4 names.
// Evidence:  calls from tSfcApp3d and the fleet entry (original name and file unknown)

#ifndef fn_0090bed4_Object_h
#define fn_0090bed4_Object_h 1

#include <list>

class tObject_005235a7;
class tCommandDestination;
class tObject_0051c7af;
class tAIData;

// HYPOTHESIS: the object at +0x218 of the object below; 0x00523100 forwards to its 0x00520e30.
// Its size is unknown.
class tObject_00520e30 {
public:
	void fn_00520e30();
};

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
	bool fn_00523027(int param_1, int param_2);
	bool fn_00523086(int param_1, int param_2);
	bool fn_005230dd(int param_1, int param_2, int param_3, int param_4);
	tObject_0051c7af *fn_00523543(int param_1);
	tObject_0051c7af *fn_00523574(int param_1);
	void fn_00523100();
	char fn_0052310b(int param_1, int param_2, int param_3);
	char fn_0052315a(void *param_1, void *param_2, int param_3);
	void fn_00523198();
	void fn_00523222();
	tAIData *fn_0052327e(int param_1, char param_2);
	void fn_00523428(unsigned char param_1);
	char fn_00523370(int param_1, char param_2);
	void fn_00523344(unsigned char param_1);
	void fn_005233cb();

	char m_pad0000[0x218];
	tObject_00520e30 m_218;
	char m_pad0219[0x16b5 - 0x219];
	// HYPOTHESIS: a flag 0x00523222 sets.
	char m_16b5;
	char m_pad16b6[0x16b8 - 0x16b6];
	// HYPOTHESIS: two path settings 0x00523198 reads into buffers it allocates.
	char *m_16b8;
	char *m_16bc;
	// HYPOTHESIS: IDs that 0x00523027 looks up through 0x00523574.
	std::list<int> m_16c0;
	char m_pad16cc[0x16d0 - 0x16cc];
	// HYPOTHESIS: a factor and two settings 0x005233cb picks from the count below.
	float m_16d0;
	char m_16d4;
	char m_16d5;
	char m_pad16d6[0x16d8 - 0x16d6];
	// HYPOTHESIS: a count of the used entries of the table below.
	unsigned int m_16d8;
	// HYPOTHESIS: a 100-byte table 0x00523222 fills with 0xff.
	char m_16dc[100];
};

extern fn_0090bed4_Object *g_p0090bed4;

#endif // fn_0090bed4_Object_h
