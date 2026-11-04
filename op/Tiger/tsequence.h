//	tsequence.h: generic interface of a number sequence generator.

#ifndef tsequence_h
#define tsequence_h 1

// The original includes tbasic.h here. The unit map has no place for a header without a unit of
// its own yet, so the declarations this unit needs from it are kept here under its guard.
#ifndef	tbasic_h
#define	tbasic_h	1

typedef	signed long		int32;
typedef	signed long		int32s;
typedef	unsigned long	int32u;

typedef	signed short	int16;
typedef	signed short	int16s;
typedef	unsigned short	int16u;

typedef	signed char		int8;
typedef	signed char		int8s;
typedef unsigned char	int8u;

typedef	int32s			iCount;
typedef	int32s			iIndex;

typedef	int32u			iTag;
typedef	int8u			iTruth;

#define	kFalse	0
#define	kTrue	1

#ifndef NULL
#define NULL    0
#endif

class	GenericException {};
#ifdef assert
#undef assert
#endif
#define	assert(x)	{ if (!(x)) {throw GenericException();} }

#endif	// end tbasic_h block

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
