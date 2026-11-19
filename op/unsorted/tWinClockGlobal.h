// unsorted/tWinClockGlobal.h

#if !defined( tWinClockGlobal_h )
#define tWinClockGlobal_h

#include "Common/AutoPtr.h"

// HYPOTHESIS: the original header is not known. The static auto_ptr's symbol is
// ?fpAutoWinClockGlobalPtr@tWinClockGlobal@@0V?$auto_ptr@VtWinClockGlobal@@@std@@A, so the class
// holds it as a private static member; the rest of the class is not declared yet.
class tWinClockGlobal
{
public:
	static tWinClockGlobal* Instance();

private:
	typedef std::auto_ptr<tWinClockGlobal> iAutoWinClockGlobalPtr;
	friend iAutoWinClockGlobalPtr;

	static iAutoWinClockGlobalPtr fpAutoWinClockGlobalPtr;
};

#endif // !defined( tWinClockGlobal_h )
