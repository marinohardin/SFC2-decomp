//	tcrandsequence.h: the standard C random number generator and a linear congruential one,
//	as a tSequencePart.

#ifndef tcrandsequence_h
#define tcrandsequence_h 1

#include "Tiger/tsequence.h"


class tCRandomSequence : public tSequencePart
{
public:

	typedef enum
	{
		kStandardCAlgorithm,
		kLinearCongruentialAlgorithm
	} tGenerator;

	tCRandomSequence(int32u initialSeed = 1, int32u historySize = 0);

	tCRandomSequence(int8u bitSize, int32u initialSeed = 1, int32u historySize = 0);

	tCRandomSequence(tGenerator algorithm, int32u initialSeed = 1, int32u historySize = 0);

	tCRandomSequence(int32u initialSeed, int32u historySize, tGenerator algorithm, int8u bitSize);

	~tCRandomSequence(void);

	tGenerator	mGetGenerator(void)					{ return fAlgorithm; }

	int32u		mGetRandomInteger(int32 upper_limit) { return (int32)(upper_limit*(double)mQuickGet()/(double)fLargestValue); };

	double		mGetRandomDouble() { return (double)mQuickGet()/(double)fLargestValue; };
	float		mGetRandomFloat() { return (float)((double)mQuickGet()/(double)fLargestValue); };

	int32u		mGetLargestValue() { return fLargestValue; };

	virtual void	mSetSeed(int32u newSeed);

	int32u mGetLastValueGenerated() { return fLastValueGenerated; };

protected:

	tGenerator	fAlgorithm;
	int8u		fBitSize;
	int32u		fLargestValue;
	int32u		fLastValueGenerated;
	int32u		fModulusM;
	int32u		fMultiplierA;
	int32u		fIncrementC;

	virtual int32u	mGenerateNumber(void);
};

#endif
