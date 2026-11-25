//	tAIData.h: the class whose constructor sits at 0x00520966.
// Evidence:  the analysis names 0x00520966 tAIData::tAIData (original file unknown)

#ifndef tAIData_h
#define tAIData_h 1

// 0x20 bytes (0x0052327e allocates that much). The vtable is at 0x007b018c and its slots 1, 3, 4, 6
// and 7 are the tETrait ones (0x00520990, 0x004bd431, 0x0052099d, 0x00436663, 0x0043666d), so
// the class derives from tETrait: its three dwords at +0x4 to +0xc are cleared by the constructor
// 0x00520966, the one at +0xc being what slot 6 stores. tETrait has no header yet (HEADER REQUEST
// in the notes), so they are declared here. The destructor at 0x005209eb stores the tETrait vtable
// 0x007b01b0, calls the base destructor and frees the pointer at +0x18 (through 0x005288cb) if set.
class tAIData {
public:
	// HYPOTHESIS: the char is the slot 0x00523370 returns, the int an entity ID.
	tAIData(char param_1, int param_2);

	virtual ~tAIData();                    // slot 0, 0x005209a3 (scalar deleting destructor)
	virtual void vfunction1();             // 0x00520990, shared with tETrait
	virtual void vfunction2(int param_1);  // 0x005209bf: when the global byte at 0x0096ea16 is set and the pointer at +0x18 is non-zero, calls 0x0052893f with param_1
	virtual void vfunction3();             // 0x004bd431, shared with tETrait
	virtual int vfunction4();               // 0x0052099d, returns 1
	virtual const char *vfunction5();      // 0x005209db, returns the string "AIData"
	virtual void vfunction6(int param_1);  // 0x00436663, shared with tETrait: stores the argument at +0xc
	virtual void vfunction7();             // 0x0043666d, shared with tETrait

	int field_4;                           // +0x4, cleared by the constructor
	int field_8;                           // +0x8, cleared by the constructor
	int field_c;                           // +0xc, cleared by the constructor
	// HYPOTHESIS: the slot of this AI's 12-byte entry in the table at +0x16dc of its owner.
	unsigned char m_10;                    // +0x10, the constructor's char; the destructor passes it to 0x00523344
	int field_14;                          // +0x14, the constructor's int
	void *field_18;                        // +0x18, cleared by the constructor
	char field_1c;                         // +0x1c, cleared by the constructor
};

// Last in the file: a typedef between declarations once changed another function's score.
typedef char tAIData_size_is_0x20[ sizeof( tAIData ) == 0x20 ? 1 : -1 ];

#endif // tAIData_h
