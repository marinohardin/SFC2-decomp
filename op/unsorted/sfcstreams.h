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
	void mSetVersionNumber( int32 versionNumber );
	int mGetVersionNumber();

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

#endif
