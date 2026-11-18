//	tSupplyDockInfo.h
// Evidence:  class (RTTI name; original file unknown)

#ifndef tSupplyDockInfo_h
#define tSupplyDockInfo_h 1

// HYPOTHESIS: 0x58 bytes, from the allocation in tSFCPlayer's constructor.
class tSupplyDockInfo
{
	public:
		tSupplyDockInfo();

	private:
		char m_00[0x58];
};

#endif // tSupplyDockInfo_h
