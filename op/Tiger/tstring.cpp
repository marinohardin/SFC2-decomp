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

// MATCH-ONLY: the original (/Ob2) inlines tString::mExpansionRoom here and keeps its out-of-line copy below; under /Ob1 the body is written out.
inline int32 fn_ExpansionRoomInline(int32 size)
{
	if (size < 50)
		return size * 2 + 1;
	if (size < 1000)
		return size * 20 / 100;
	return size * 5 / 100;
}

// MATCH-ONLY: the original inlines a reserve with this growth rule into the copy constructor; the descriptive name is this project's own.
inline void tString::mReserveWithRoom(int32 size)
{
	if (size > 0)
	{
		int32 newSize = size + fn_ExpansionRoomInline(size);
		fInstanceControl.fMemSpace.reserve(newSize);
		fInstanceControl.fMemLimit = newSize;
	}
}

// MATCH-ONLY: the original inlines this (built with /Ob2).
inline int32 tString::mGetMemSize(void)
{
	return fInstanceControl.fMemSpace.capacity();
}

// MATCH-ONLY: the original inlines this (built with /Ob2).
inline tString& tString::operator= (const tString &s)
{
	if (mGetMemSize() != 0)
	{
		fInstanceControl.fDataCount = s.fInstanceControl.fDataCount;
		fInstanceControl.fMemSpace = s.fInstanceControl.fMemSpace;
	}
	return *this;
}

// FUNCTION: STARFLEETOP 0x00623390
tString::tString(const tString &s)
{
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;

	mReserveWithRoom(s.fInstanceControl.fDataCount);
	*this = s;
}

// FUNCTION: STARFLEETOP 0x00623500
tString::tString(const iterator &i)
{
	fInstanceControl.fTempExternalCharRepresentation = NULL;
	fInstanceControl.fSizeOfExternalCharAllocation = 0;
	fInstanceControl.fTempOperatorSpace = NULL;
	fInstanceControl.fMemLimit = 0;
	fInstanceControl.fDataCount = 0;

	mIncorporateCharList(&i.fString->fInstanceControl.fMemSpace[i.fIndex], i.fString->fInstanceControl.fDataCount - i.fIndex);
}

// FUNCTION: STARFLEETOP 0x00623580
tString::~tString()
{
	if (fInstanceControl.fTempExternalCharRepresentation)
		delete [] fInstanceControl.fTempExternalCharRepresentation;
	if (fInstanceControl.fTempOperatorSpace)
		delete fInstanceControl.fTempOperatorSpace;
}

// FUNCTION: STARFLEETOP 0x00623600
void tString::mReserve(int32 size)
{
	if (size > fInstanceControl.fMemLimit)
	{
		fInstanceControl.fMemSpace.reserve(size);
		fInstanceControl.fMemLimit = size;
	}
}

// FUNCTION: STARFLEETOP 0x006236b0
int32 tString::mExpansionRoom(int32 size)
{
	if (size < 50)
		return size * 2 + 1;
	if (size < 1000)
		return size * 20 / 100;
	return size * 5 / 100;
}

// FUNCTION: STARFLEETOP 0x00623700
iBasicStringElement *tString::mWideStr(void)
{
	if (fInstanceControl.fTempExternalCharRepresentation)
		delete [] fInstanceControl.fTempExternalCharRepresentation;
	int32 size = fInstanceControl.fDataCount * sizeof(iBasicStringElement) + sizeof(iBasicStringElement);
	fInstanceControl.fTempExternalCharRepresentation = new char[size];
	fInstanceControl.fSizeOfExternalCharAllocation = fInstanceControl.fDataCount * sizeof(iBasicStringElement) + sizeof(iBasicStringElement);
	if (fInstanceControl.fTempExternalCharRepresentation)
	{
		iBasicStringElement *wide = (iBasicStringElement *)memcpy(fInstanceControl.fTempExternalCharRepresentation, fInstanceControl.fMemSpace.begin(), fInstanceControl.fDataCount * sizeof(iBasicStringElement));
		wide[fInstanceControl.fDataCount] = 0;
	}
	return (iBasicStringElement *)fInstanceControl.fTempExternalCharRepresentation;
}
