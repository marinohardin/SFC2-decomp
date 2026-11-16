//	tstring.cpp: tString, Tiger's wide character string.

#include "Tiger/tstring.h"

// FUNCTION: STARFLEETOP 0x006230b0
void tString::mInitInstanceControl(void)
{
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;
}

// FUNCTION: STARFLEETOP 0x006230d0
tString::tString()
{
	// MATCH-ONLY: the original inlines mInitInstanceControl here (built with /Ob2); the
	// profile has no /Ob, so its body is written out.
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;
}

// MATCH-ONLY: the original inlines this single-argument form (built with /Ob2).
inline iTruth tString::mIncorporateCharList(const char *s)
{
	int32 length = 0;
	if (s != NULL)
	{
		char c = *s;
		while (c != '\0')
		{
			length++;
			c = s[length];
		}
	}
	return mIncorporateCharList(s, length);
}

// FUNCTION: STARFLEETOP 0x006231a0
// FUNCTION: STARFLEETOP 0x00623120
tString::tString(const char *s)
{
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;

	mIncorporateCharList(s);
}

// MATCH-ONLY: the original inlines this single-argument form (built with /Ob2).
inline iTruth tString::mIncorporateCharList(const iBasicStringElement *s)
{
	int32 length = 0;
	if (s != NULL)
	{
		const iBasicStringElement *p = s;
		while (*p != 0)
		{
			p++;
			length++;
		}
	}
	return mIncorporateCharList(s, length);
}

// FUNCTION: STARFLEETOP 0x00623260
tString::tString(const iBasicStringElement *s)
{
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;

	mIncorporateCharList(s);
}

// FUNCTION: STARFLEETOP 0x006232f0
tString::tString(const iBasicStringElement c)
{
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;

	iBasicStringElement s[2];
	s[0] = c;
	s[1] = 0;
	mIncorporateCharList(s);
}
