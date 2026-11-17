//	fn_004d290d_object.h: the 0x64-byte member at tPanelMgr+0x100 and
//	tState_SingleCampaignSelector+0x40; class and original file unknown.

#ifndef fn_004d290d_object_h
#define fn_004d290d_object_h 1

class fn_004d288f_object;

// HYPOTHESIS: the member at tPanelMgr+0x100; it is taken to end before the flag at +0x164 that
// fn_00608bd1 reads. Only its functions 0x004d290d (no argument), 0x004d288f (looks up an object
// by a pointer key) and 0x004d269b (two globals and a flag) are known.
struct fn_004d290d_object {
    ~fn_004d290d_object();	// 0x004d2636
    void fn_004d290d();
    fn_004d288f_object *fn_004d288f(const void *key);
    void fn_004d269b(void *a, void *b, bool flag);
    char m_0x00[0x64];
};

#endif	// fn_004d290d_object_h
