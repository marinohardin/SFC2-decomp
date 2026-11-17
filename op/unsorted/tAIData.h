//	tAIData.h: the class whose constructor sits at 0x00520966.
// Evidence:  the analysis names 0x00520966 tAIData::tAIData (original file unknown)

#ifndef tAIData_h
#define tAIData_h 1

// HYPOTHESIS: 0x20 bytes (0x0052327e allocates that much); only the byte at +0x10 is known.
class tAIData {
public:
	// HYPOTHESIS: the char is the slot 0x00523370 returns, the int an entity ID.
	tAIData(char param_1, int param_2);

	char m_pad0000[0x10];
	// HYPOTHESIS: the slot of this AI's 12-byte entry in the table at +0x16dc of its owner.
	unsigned char m_10;
	char m_pad0011[0x20 - 0x11];
};

#endif // tAIData_h
