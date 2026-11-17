//	sfcstreams.h
//
//	The byte stream the game archives and sends its data through. Declarations follow the
//	SDK's header; only what this unit's functions need is declared yet.
//
#pragma once

#if !defined( sfcstreams_h )
#define sfcstreams_h

#include "port/types.h"

typedef	int8u			iTruth;

struct tPoint3;
struct tPoint2;
class tString;

class tSFCStream
{
public:
	tSFCStream(int32 x);
	void fn_0040d67d();
	void fn_0040d692();
	tSFCStream* fn_0040d6b7(int* src);
	tSFCStream* fn_0040d705(int* src);
	tSFCStream* fn_0040d73e(tString* str);
	// HYPOTHESIS: 0x0040d867 is a member of this buffer class taking (data, size, count).
	void fn_0040d867(void* data, int size, int count);
	// HYPOTHESIS: 0x0040d897 is the read counterpart of 0x0040d867.
	int fn_0040d897(void* data, int size, int count);
	void fn_0040d8cf(int n);
	void fn_0040d8e8(int8u* p);
	tSFCStream* fn_0040d7d6(tString* str);
	void fn_0040d815();
	void fn_0040d841();
	tSFCStream& operator>>(tPoint3& num);
	tSFCStream& operator>>(tPoint2& num);
	// HYPOTHESIS: 0x0040e6d1 writes a zero-terminated string (it takes strlen + 1).
	void fn_0040e6d1(char* s);
	int fn_0040e699(char* p);
	// HYPOTHESIS: 0x0040e744 is a member taking a formatted text line.
	void fn_0040e744(char* text);
	// HYPOTHESIS: 0x0040e70f is the read counterpart of 0x0040e744 (fetches a text line).
	int fn_0040e70f(char* text);
	void mIncrementTabNumber();
	void mDecrementTabNumber();
	int8u *mGetReadPtr() const	{ return fGetPtr; }
	int8u *mGetWritePtr() const	{ return fPutPtr; }
	int32 mGetReadSize() const { return (int32)(fPutPtr-fGetPtr); }
	void mSetReadSize(int32 readSize);
	void mSetVersionNumber( int32 versionNumber );
	int mGetVersionNumber();
	void mWriteStringToStream(char *str);

	//Generic Stream function
	static inline void mIO(void *p, const void *src, int size);

protected:
	int32 fVersionNumber;
	ptr32( int8u ) fBuffer;
	ptr32( int8u ) fGetPtr;
	ptr32( int8u ) fPutPtr;
	ptr32( int8u ) fMaxPtr;
	iTruth fOwned;
	iTruth	fReadLock;
	iTruth	fWriteLock;

	int32 fTabNumber;
};

inline void tSFCStream::mIncrementTabNumber()
{
	fTabNumber++;
}

inline void tSFCStream::mDecrementTabNumber()
{
	fTabNumber--;
}

inline void tSFCStream::mIO(void *p, const void *src, int size)
{
	const int8u	*s = (int8u*)src;
	int8u		*d = (int8u*)p;

	for (int i=0; i<size; i++)
	{
		*d++ = s[0];
		s++;
	}
}

#endif
