//	tsequence.h: generic interface of a number sequence generator.

#ifndef tsequence_h
#define tsequence_h 1

#include "Tiger/tbasic.h"

class tSequencePart
{
protected:

	int32u fNumberCount;

	const int32u kHistorySize;
	int32u *fHistory;

	int32u fSeed;
	int32u fPreviousResult;

	virtual int32u		mGenerateNumber(void)		= 0;

public:

	tSequencePart(int32u initialSeed = 1, int32u historySize = 0)
		: fSeed(initialSeed), kHistorySize(historySize), fNumberCount(0)
	{
		if (kHistorySize > 0)
		{
			fHistory = new int32u[kHistorySize];
		}
		else
		{
			fHistory = NULL;
		}

		fPreviousResult = fSeed;
	};

	virtual ~tSequencePart()
	{
		if (fHistory)
		{
			delete [] fHistory;
		}
	};

	virtual void	mSetSeed(int32u newSeed)
	{
		fSeed = newSeed;
		fPreviousResult = newSeed;
	}

	virtual int32u	mGetSeed(void) { return fSeed; }

	virtual int32u	mGetNumber(void);

	virtual int32u	mGetNumber(int32u index);

	virtual int32u	mQuickGet(void) { return mGenerateNumber(); }

};

#endif
