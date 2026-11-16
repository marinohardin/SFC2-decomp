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

// FUNCTION: STARFLEETOP 0x00623d30
tString tString::substr(int32 start)
{
	return substr(start, mGetDataCount() - start);
}

// FUNCTION: STARFLEETOP 0x00623d60
tString tString::substr(int32 start, int32 length)
{
	tString result;
	if (length == -1)
		length = mGetDataCount();
	if (start < 0)
		start = 0;
	if (length < 0)
		length = 0;
	if (start + length > mGetDataCount())
		length = mGetDataCount() - start;
	result.mIncorporateCharList(&fInstanceControl.fMemSpace[start], length);
	return result;
}

// FUNCTION: STARFLEETOP 0x00623ed0
iBasicStringElement& tString::operator[](int32 n)
{
	if (n < 0)
		throw "Invalid index for dereference of tString";
	if (n > fInstanceControl.fDataCount)
		throw "Invalid index for dereference of tString";
	return fInstanceControl.fMemSpace[n];
}

// FUNCTION: STARFLEETOP 0x00623f20
int32 tString::find(char c)
{
	char s[2];
	s[0] = c;
	s[1] = 0;
	return find(s);
}

// FUNCTION: STARFLEETOP 0x00623f40
int32 tString::find(const char *s, int32 startPos)
{
	return find(tString(s), startPos);
}

// MATCH-ONLY: the original inlined these small members into find.
inline int32 tString::size() const
{
	return fInstanceControl.fDataCount;
}

inline tString::iterator tString::begin()
{
	return iterator(0, this);
}

inline tString::iterator tString::end()
{
	return iterator(fInstanceControl.fDataCount, this);
}
// FUNCTION: STARFLEETOP 0x00625400
iBasicStringElement& tString::iterator::operator*()
{
	if (!fString)
		throw "Access to uninitialized iterator";
	return fString->fInstanceControl.fMemSpace[fIndex];
}

inline tString::iterator tString::iterator::operator++()
{
	iterator old = *this;
	if (fIndex + 1 > fString->fInstanceControl.fDataCount)
		throw "Access past bounds of tString";
	fIndex++;
	return old;
}

// FUNCTION: STARFLEETOP 0x00624020
int32 tString::find(tString &s, int32 startPos)
{
	int32 &pos = startPos;
	if (pos < 0)
		pos = 0;
	int32 count = size() - s.size() - pos + 1;
	if (count <= 0)
		return -1;
	{
		iterator b = begin();
		// MATCH-ONLY: the original (/Ob2) inlines iterator::operator+ (begin() + pos) here.
		if (!b.fString)
			throw "Access to uninitialized iterator";
		iterator t(b);
		t.fIndex = b.fIndex + pos;
		if (t.fIndex < 0 || t.fIndex > b.fString->fInstanceControl.fDataCount)
			throw "Access past bounds of tString";
		iterator i(t);
		for (;;)
		{
			iterator a = i;
			iterator j = s.begin();
			iTruth match;
			do
			{
				if (!(j != s.end()))
					return pos;
				// MATCH-ONLY: the original (/Ob2) inlines iterator::operator* here and keeps its
				// out-of-line copy; under /Ob1 its body is written out.
				if (!a.fString)
					throw "Access to uninitialized iterator";
				if (!j.fString)
					throw "Access to uninitialized iterator";
				match = (a.fString->fInstanceControl.fMemSpace[a.fIndex] == j.fString->fInstanceControl.fMemSpace[j.fIndex]);
				++j;
				++a;
			} while (match);
			pos++;
			if (--count == 0)
				break;
			i++;
		}
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x00624260
int32 tString::rfind(const char *s, int32 startPos)
{
	return rfind(tString(s), startPos);
}

// FUNCTION: STARFLEETOP 0x00624340
int32 tString::rfind(const char *s)
{
	tString t(s);
	return rfind(t, size());
}

// FUNCTION: STARFLEETOP 0x00624420
int32 tString::rfind(tString &s, int32 startPos)
{
	int32 otherSize = s.size();
	int32 mySize = size();
	int32 pos = startPos;
	if (mySize - pos < otherSize)
		pos = mySize - otherSize;
	int32 count = pos + 1;
	if (count <= 0)
		return -1;
	{
		iterator b = begin();
		// MATCH-ONLY: the original (/Ob2) inlines iterator::operator+ (begin() + pos) here.
		if (!b.fString)
			throw "Access to uninitialized iterator";
		iterator t(b);
		t.fIndex = b.fIndex + pos;
		if (t.fIndex < 0 || t.fIndex > b.fString->fInstanceControl.fDataCount)
			throw "Access past bounds of tString";
		iterator i(t);
		for (;;)
		{
			iterator a = i;
			iterator j = s.begin();
			iTruth match;
			do
			{
				if (!(j != s.end()))
					return pos;
				// MATCH-ONLY: the original (/Ob2) inlines iterator::operator* here, as in find.
				if (!j.fString)
					throw "Access to uninitialized iterator";
				if (!a.fString)
					throw "Access to uninitialized iterator";
				match = (j.fString->fInstanceControl.fMemSpace[j.fIndex] == a.fString->fInstanceControl.fMemSpace[a.fIndex]);
				++j;
				++a;
			} while (match);
			pos--;
			if (--count == 0)
				break;
			i--;
		}
	}
	return -1;
}

// FUNCTION: STARFLEETOP 0x00624660
void tString::operator=(const char* s)
{
	mIncorporateCharList(s);
}

// FUNCTION: STARFLEETOP 0x00624b60
tString& tString::operator+=(const tString &s)
{
	if (fInstanceControl.fDataCount + s.fInstanceControl.fDataCount < 50000)
	{
		int32 newCount = fInstanceControl.fDataCount + s.fInstanceControl.fDataCount;
		if (newCount > fInstanceControl.fMemLimit)
		{
			newCount += fn_ExpansionRoomInline(newCount);
			fInstanceControl.fMemLimit = newCount;
			fInstanceControl.fMemSpace.reserve(newCount);
		}
		fInstanceControl.fDataCount += s.fInstanceControl.fDataCount;
		if (mGetMemSize() != 0)
			fInstanceControl.fMemSpace.insert(fInstanceControl.fMemSpace.end(), s.fInstanceControl.fMemSpace.begin(), s.fInstanceControl.fMemSpace.end());
	}
	return *this;
}

// FUNCTION: STARFLEETOP 0x00624df0
tString& tString::operator+=(const iBasicStringElement &c)
{
	if (fInstanceControl.fDataCount + 1 < 50000)
	{
		if (fInstanceControl.fDataCount + 1 > fInstanceControl.fMemLimit)
		{
			int32 newCount = fInstanceControl.fDataCount + 1;
			newCount += fn_ExpansionRoomInline(newCount);
			fInstanceControl.fMemSpace.reserve(newCount);
			fInstanceControl.fMemLimit = newCount;
		}
		fInstanceControl.fDataCount++;
		if (mGetMemSize() != 0)
			// HYPOTHESIS: the original's insert stays out of line (0x0071b990); here VC6 inlines it.
		{
			// MATCH-ONLY: keeps vector::insert out of line, as the original calls it
			typedef std::vector<iBasicStringElement> tVec;
			void (tVec::*pInsert)(tVec::iterator, tVec::size_type, const iBasicStringElement &) = &tVec::insert;
			tVec &v = fInstanceControl.fMemSpace;
			(v.*pInsert)(v.end(), 1, c);
		}
	}
	return *this;
}

// FUNCTION: STARFLEETOP 0x00624f20
iTruth operator== (const tString &s1,const tString &s2)
{
	if (s1.fInstanceControl.fDataCount != s2.fInstanceControl.fDataCount)
		return false;
	return memcmp(s1.fInstanceControl.fMemSpace.begin(), s2.fInstanceControl.fMemSpace.begin(), s1.fInstanceControl.fDataCount * sizeof(iBasicStringElement)) == 0;
}

// FUNCTION: STARFLEETOP 0x00624f50
iTruth operator<  (const tString &s1,const tString &s2)
{
	int n = s1.fInstanceControl.fDataCount < s2.fInstanceControl.fDataCount ? s1.fInstanceControl.fDataCount : s2.fInstanceControl.fDataCount;
	int c = memcmp(s1.fInstanceControl.fMemSpace.begin(), s2.fInstanceControl.fMemSpace.begin(), n * sizeof(iBasicStringElement));
	if (c == 0 && s1.fInstanceControl.fDataCount < s2.fInstanceControl.fDataCount)
		return true;
	return c < 0 ? true : false;
}

// FUNCTION: STARFLEETOP 0x00624fa0
// HYPOTHESIS: the original (/Ob2) inlines tString(const char *) and ~tString here; with the
// constructor marked inline this reaches 99.25%, but 0x00623120 is then never emitted.
tString& tString::operator+(const char *s)
{
	return *this + tString(s);
}

// FUNCTION: STARFLEETOP 0x00625080
tString& tString::operator+=(const char *s)
{
	return *this += tString(s);
}

// FUNCTION: STARFLEETOP 0x00625160
void tString::operator=(const iBasicStringElement* s)
{
	mIncorporateCharList(s);
}

// HYPOTHESIS: no out-of-line copy is known in the exe; the original inlines it into the
// comparison operators below.
inline int16 mCStrCompare(const tString &s1, const char* s2)
{
	const iBasicStringElement *p = s1.fInstanceControl.fMemSpace.begin();
	const char *c = s2;
	int16 r = 0;
	int i = 0;
	while (r == 0 && *c && i < s1.fInstanceControl.fDataCount)
	{
		r = *p++ - (unsigned char)*c++;
		i++;
	}
	if (r == 0)
	{
		if (i == s1.fInstanceControl.fDataCount && *c)
			r = -1;
		else if (i < s1.fInstanceControl.fDataCount && !*c)
			r = 1;
	}
	return r;
}

// FUNCTION: STARFLEETOP 0x00625190
iTruth operator== (const tString &s1,const char *s2)
{
	return 0 == mCStrCompare(s1, s2);
}

// FUNCTION: STARFLEETOP 0x00625210
iTruth operator!= (const tString &s1,const char *s2)
{
	return 0 != mCStrCompare(s1, s2);
}

// FUNCTION: STARFLEETOP 0x00625360
int32s tString::mGetInt32s(void)
{
	return StringToNum(mCharStr());
}

// MATCH-ONLY: the original (/Ob2) inlines the empty default constructor at every use.
inline tString::iterator::iterator()
{
}

// FUNCTION: STARFLEETOP 0x00625380
tString::iterator::iterator(const iterator &i)
{
	fIndex = i.fIndex;
	fString = i.fString;
}

// FUNCTION: STARFLEETOP 0x006253a0
tString::iterator::iterator(int32 index, tString *s)
{
	fIndex = index;
	fString = s;
	if (!fString)
		throw "Access to uninitialized iterator";
	if (fIndex < 0 || fIndex > fString->fInstanceControl.fDataCount)
		throw "Access past bounds of tString";
}

// FUNCTION: STARFLEETOP 0x00625430
tString::iterator tString::iterator::operator+(int32 n)
{
	if (!fString)
		throw "Access to uninitialized iterator";
	if (fIndex + n < 0 || fIndex + n > fString->fInstanceControl.fDataCount)
		throw "Access past bounds of tString";
	return iterator(fIndex + n, fString);
}

// FUNCTION: STARFLEETOP 0x00625490
tString::iterator tString::iterator::operator-(int32 n)
{
	if (!fString)
		throw "Access to uninitialized iterator";
	int32 index = fIndex - n;
	if (index < 0 || index > fString->fInstanceControl.fDataCount)
		throw "Access past bounds of tString";
	iterator r;
	r.fIndex = index;
	r.fString = fString;
	return r;
}

// FUNCTION: STARFLEETOP 0x006254f0
tString::iterator tString::iterator::operator++(int)
{
	if (!fString)
		throw "Access to uninitialized iterator";
	int32 index = fIndex;
	if (index + 1 > fString->fInstanceControl.fDataCount)
		throw "Access past bounds of tString";
	fIndex = index + 1;
	return iterator(index, fString);
}

// FUNCTION: STARFLEETOP 0x00625550
tString::iterator tString::iterator::operator--(int)
{
	if (!fString)
		throw "Access to uninitialized iterator";
	int32 index = fIndex;
	if (index - 1 < 0)
		throw "Access past bounds of tString";
	fIndex = index - 1;
	return iterator(index, fString);
}
