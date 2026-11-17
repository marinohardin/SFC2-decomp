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

class tSFCStream
{
public:
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
