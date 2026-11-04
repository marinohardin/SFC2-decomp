//	tsequence.cpp: tSequencePart, the generic number sequence generator.

#include "Tiger/tsequence.h"


// FUNCTION: STARFLEETOP 0x00670180
int32u tSequencePart::mGetNumber(void)
{
	int32u result = mGenerateNumber();

	fNumberCount++;

	if (fHistory)
	{
		for (int32u i = kHistorySize - 1; i > 0; i--)
		{
			fHistory[i] = fHistory[i - 1];
		}

		fHistory[0] = result;
	}

	return result;
}

// FUNCTION: STARFLEETOP 0x006701c0
int32u tSequencePart::mGetNumber(int32u index)
{
	if (fHistory && (index <= fNumberCount) && (index >= 1))
	{
		if ((fNumberCount - index) > (kHistorySize - 1))
		{
			fPreviousResult = fSeed;

			for (int32u i = 1; i < index; i++)
			{
				mGenerateNumber();
			}

			return mGenerateNumber();
		}

		return fHistory[fNumberCount - index];
	}

	return 0;
}
