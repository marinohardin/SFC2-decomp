//	tcrandsequence.cpp: tCRandomSequence, the standard C random number generator and a linear
//	congruential one.

#include "Tiger/tcrandsequence.h"

#include <stdlib.h>


// FUNCTION: STARFLEETOP 0x0066c080
tCRandomSequence::tCRandomSequence(int32u initialSeed, int32u historySize)
	: tSequencePart(initialSeed, historySize)
{
	fLastValueGenerated = initialSeed;
	fAlgorithm = kStandardCAlgorithm;
	fLargestValue = RAND_MAX + 1;
}

// FUNCTION: STARFLEETOP 0x0066c100
tCRandomSequence::tCRandomSequence(int8u bitSize, int32u initialSeed, int32u historySize)
	: tSequencePart(initialSeed, historySize)
{
	if ((bitSize <= 31) && (bitSize >= 8))
	{
		fBitSize = bitSize;
		fModulusM = (1 << bitSize) - 1;
	}
	else
	{
		fBitSize = 31;
		fModulusM = 0x7FFFFFFF;
	}

	fLastValueGenerated = initialSeed;
	fMultiplierA = 201;
	fAlgorithm = kLinearCongruentialAlgorithm;
	fIncrementC = (1 << (fBitSize - 1)) + 1;
	fLargestValue = fModulusM + 1;
}

// FUNCTION: STARFLEETOP 0x0066c1a0
tCRandomSequence::~tCRandomSequence(void)
{
}

// FUNCTION: STARFLEETOP 0x0066c1c0
void tCRandomSequence::mSetSeed(int32u newSeed)
{
	fSeed = newSeed;
	srand(newSeed);
	fLastValueGenerated = newSeed;
}

// FUNCTION: STARFLEETOP 0x0066c1e0
int32u tCRandomSequence::mGenerateNumber(void)
{
	switch (fAlgorithm)
	{
		case kStandardCAlgorithm:
			fLastValueGenerated = rand();
			break;

		case kLinearCongruentialAlgorithm:
			fLastValueGenerated = (fMultiplierA * fLastValueGenerated + fIncrementC) & fModulusM;
			break;
	}

	fPreviousResult = fLastValueGenerated;

	return fLastValueGenerated;
}
