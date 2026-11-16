//	tstring.cpp: tString, Tiger's wide character string.
#include "Tiger/tstring.h"
#include "Tiger/tcglue.h"

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

// MATCH-ONLY: the original inlines this (built with /Ob2).
inline int32 tString::mGetMemSize(void)
{
	return fInstanceControl.fMemSpace.capacity();
}

// MATCH-ONLY: the original inlines this (built with /Ob2).
inline int32 tString::mGetDataCount(void)
{
	return fInstanceControl.fDataCount;
}

// MATCH-ONLY: the original inlines this (built with /Ob2).
// HYPOTHESIS: the growth reserve belongs to operator=; the copy constructor and mEndStrAt both inline it.
inline tString& tString::operator=(const tString &s)
{
	if (fInstanceControl.fMemLimit < s.fInstanceControl.fDataCount)
	{
		int32 newSize = s.fInstanceControl.fDataCount + fn_ExpansionRoomInline(s.fInstanceControl.fDataCount);
		fInstanceControl.fMemSpace.reserve(newSize);
		fInstanceControl.fMemLimit = newSize;
	}
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

// MATCH-ONLY: mEndStrAt inlines the destructor of its temporary (built with /Ob2); this copy is the vtable's.
// FUNCTION: STARFLEETOP 0x00623580
inline tString::~tString()
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

// FUNCTION: STARFLEETOP 0x00623760
char* tString::mCharStr( char* myMemory )
{
	if (myMemory)
	{
		char *p = myMemory;
		for (int32 i = 0; i < fInstanceControl.fDataCount; i++)
			*p++ = (char)fInstanceControl.fMemSpace[i];
		*p = 0;
		return myMemory;
	}
	if (fInstanceControl.fTempExternalCharRepresentation)
		delete [] fInstanceControl.fTempExternalCharRepresentation;
	int32 size = fInstanceControl.fDataCount + 1;
	fInstanceControl.fTempExternalCharRepresentation = new char[size];
	fInstanceControl.fSizeOfExternalCharAllocation = fInstanceControl.fDataCount + 1;
	if (fInstanceControl.fTempExternalCharRepresentation)
	{
		char *p = fInstanceControl.fTempExternalCharRepresentation;
		for (int32 i = 0; i < fInstanceControl.fDataCount; i++)
			*p++ = (char)fInstanceControl.fMemSpace[i];
		*p = 0;
	}
	return fInstanceControl.fTempExternalCharRepresentation;
}

// FUNCTION: STARFLEETOP 0x006237f0
int tString::mCopyCharStr( char *str, int max_size )
{
	int32 count = 0;
	if (max_size > 0)
	{
		char *p = str;
		iBasicStringElement *src = fInstanceControl.fMemSpace.begin();
		count = (max_size - 1 < (int32)fInstanceControl.fMemSpace.size()) ? max_size - 1 : (int32)fInstanceControl.fMemSpace.size();
		for (int32u i = 0; i < (int32u)count; i++)
			*p++ = (char)*src++;
		str[count] = 0;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x00623850
int tString::mCopyWideStr( iWideChar *str, int max_size )
{
	int32 count = 0;
	if (max_size > 0)
	{
		iBasicStringElement *src = fInstanceControl.fMemSpace.begin();
		count = (max_size - 1 < (int32)fInstanceControl.fMemSpace.size()) ? max_size - 1 : (int32)fInstanceControl.fMemSpace.size();
		clib::memcpy(str, src, count * sizeof(iWideChar));
		str[count] = 0;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x006238b0
iTruth tString::mIncorporateCharList(const iBasicStringElement *s, int32 lengthToCopy)
{
	if (s != NULL && lengthToCopy < 50000)
	{
		fInstanceControl.fMemSpace.erase(fInstanceControl.fMemSpace.begin(), fInstanceControl.fMemSpace.end());
		if (fInstanceControl.fMemLimit < lengthToCopy)
		{
			int32 newSize = lengthToCopy + fn_ExpansionRoomInline(lengthToCopy);
			fInstanceControl.fMemSpace.reserve(newSize);
			fInstanceControl.fMemLimit = newSize;
		}
		fInstanceControl.fDataCount = lengthToCopy;
		for (int32 i = 0; i < lengthToCopy; i++)
		{
			fInstanceControl.fMemSpace.push_back(*s);
			s++;
		}
	}
	return kFalse;
}

// FUNCTION: STARFLEETOP 0x00623a10
iTruth tString::mIncorporateCharList(const char *s, int32 lengthToCopy)
{
	if (s != NULL && lengthToCopy < 50000)
	{
		fInstanceControl.fMemSpace.erase(fInstanceControl.fMemSpace.begin(), fInstanceControl.fMemSpace.end());
		if (fInstanceControl.fMemLimit < lengthToCopy)
		{
			int32 newSize = lengthToCopy + fn_ExpansionRoomInline(lengthToCopy);
			fInstanceControl.fMemSpace.reserve(newSize);
			fInstanceControl.fMemLimit = newSize;
		}
		fInstanceControl.fDataCount = lengthToCopy;
		for (int32 i = 0; i < lengthToCopy; i++)
		{
			fInstanceControl.fMemSpace.push_back((unsigned char)*s);
			s++;
		}
	}
	return kFalse;
}

// FUNCTION: STARFLEETOP 0x00623b80
void tString::mEndStrAt(iterator strPosition)
{
	if (this != strPosition.fString)
		throw "You can't use someone else's iterator to terminate yourself!";
	mEndStrAt(strPosition.fIndex);
}

// FUNCTION: STARFLEETOP 0x00623bb0
void tString::mEndStrAt(int32 strPosition)
{
	if (strPosition < mGetDataCount())
	{
		*this = substr(0, strPosition);
	}
}
