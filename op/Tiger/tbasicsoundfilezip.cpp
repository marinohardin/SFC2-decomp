//	tbasicsoundfilezip.cpp: tSndZipInfoMgr, the zip directory of the sound files.

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "Tiger/tstring.h"

#include "Tiger/tsound_fileio.h"

int		QsortCompare( const void *, const void * );

// HYPOTHESIS: the zip records, packed as they are on disk (the end record is copied as 22
// bytes and the directory fields are read at odd offsets).
#pragma pack( push, 1 )
struct tSndZipEndHdr
{
	char			fSignature[4];
	unsigned short	fDisk;
	unsigned short	fDirDisk;
	unsigned short	fNumDiskEntries;
	unsigned short	fNumEntries;
	unsigned long	fDirSize;
	unsigned long	fDirOffset;
	unsigned short	fCommentLength;
};

struct tSndZipDirHdr
{
	char			fSignature[4];
	unsigned short	fVersionMade;
	unsigned short	fVersionNeeded;
	unsigned short	fFlags;
	unsigned short	fMethod;
	unsigned short	fTime;
	unsigned short	fDate;
	unsigned long	fCRC;
	unsigned long	fCompressedSize;
	unsigned long	fSize;
	unsigned short	fNameLength;
	unsigned short	fExtraLength;
	unsigned short	fCommentLength;
	unsigned short	fDiskStart;
	unsigned short	fInternalAttr;
	unsigned long	fExternalAttr;
	unsigned long	fLocalOffset;
};

struct tSndZipLocalHdr
{
	char			fSignature[4];
	unsigned short	fVersionNeeded;
	unsigned short	fFlags;
	unsigned short	fMethod;
	unsigned short	fTime;
	unsigned short	fDate;
	unsigned long	fCRC;
	unsigned long	fCompressedSize;
	unsigned long	fSize;
	unsigned short	fNameLength;
	unsigned short	fExtraLength;
};
#pragma pack( pop )

// HYPOTHESIS: layout from the constructor and mInit; names of the fields are not known.
class tSndZipInfoMgr
{
public:
	tSndZipInfoMgr();

	unsigned char	mInit( const char * );
	void			mDone();

protected:
	class tSndZipEntry
	{
		friend class tSndZipInfoMgr;

	protected:
		tSndZipEntry();
		~tSndZipEntry();

	public:
		char			*field_0;	// lower-case name, '/' separated
		unsigned long	field_4;	// file offset of the data
		unsigned long	field_8;	// compressed size
		unsigned long	field_c;	// size
		unsigned long	field_10;	// compression method
	};

	tSndZipEntry	*field_0;
	long			field_4;
	char			field_8[260];
	long			field_10c;
	tString			field_110[8];
	long			field_250;
};


// FUNCTION: STARFLEETOP 0x0066c970
tSndZipInfoMgr::tSndZipInfoMgr()
{
	field_0 = 0;
	field_4 = 0;
	field_10c = 0;
	field_250 = 0;
}

inline unsigned char SndGetDirectoryEndHdr( tBasicFile *pFile, tSndZipEndHdr *pHdr )
{
	static char kSig[4] = { 'P', 'K', 5, 6 };

	long size = pFile->mFileSize();
	if ( size < 22 )
		return 0;

	long n = 0x11a;
	if ( size < n )
		n = size;
	long base = size - n;
	pFile->mSeekEnd( -n );

	char buf[0x11a];
	if ( pFile->mRead( buf, n ) != (unsigned long)n )
		return 0;

	char *p = buf;
	char *pEnd = buf + n - 4;
	while ( p < pEnd )
	{
		if ( p[0] != kSig[0] )
		{
			p++;
			continue;
		}
		if ( p[1] == kSig[1] && p[2] == kSig[2] && p[3] == kSig[3] )
			break;
		p++;
	}
	if ( p >= pEnd )
		return 0;

	tSndZipEndHdr hdr;
	memcpy( &hdr, p, sizeof( hdr ) );
	if ( hdr.fDisk != 0 || hdr.fDirDisk != 0 || hdr.fNumDiskEntries != hdr.fNumEntries )
		return 0;
	if ( hdr.fDirSize > (unsigned long)( size - 22 ) || hdr.fDirOffset > (unsigned long)( size - 22 ) )
		return 0;
	long hdrPos = ( p - buf ) + base;
	if ( hdrPos + 22 + hdr.fCommentLength > size )
		return 0;

	memcpy( pHdr, p, sizeof( *pHdr ) );
	return 1;
}

// FUNCTION: STARFLEETOP 0x0066c9b0
unsigned char tSndZipInfoMgr::mInit( const char *pName )
{
	static const char *assets_zip = "{ii\x7fni4`sj\x1a";

	tBasicFile			*pFile = 0;	// HYPOTHESIS: held as the plain file interface (its two-argument mRead)
	long				dirPos = 0;
	char				*pDir = 0;

	try
	{
		char name[60];
		if ( pName == 0 )
		{
			const char *src = assets_zip;
			char *dst = name;
			char c;
			do
			{
				c = *src ^ 0x1a;
				*dst = c;
				dst++;
				src++;
			} while ( c );
			pName = name;
		}

		pFile = _NewSoundFileIO();
		if ( pFile == 0 )
			throw (unsigned char)0;
		if ( !pFile->mOpenForRead( pName ) )
			throw (unsigned char)0;
		strcpy( field_8, pName );

		tSndZipEndHdr endHdr;
		if ( !SndGetDirectoryEndHdr( pFile, &endHdr ) )
			throw (unsigned char)0;

		field_4 = endHdr.fNumDiskEntries;
		field_0 = new tSndZipEntry[field_4];
		if ( field_0 == 0 )
			throw (unsigned char)0;

		if ( !pFile->mSeekSet( endHdr.fDirOffset ) )
			throw (unsigned char)0;
		unsigned long dirSize = endHdr.fDirSize;
		pDir = new char[dirSize];
		if ( pDir == 0 )
			throw (unsigned char)0;
		if ( pFile->mRead( pDir, dirSize ) != dirSize )
			throw (unsigned char)0;

		for ( long i = 0; i < field_4; i++ )
		{
			tSndZipDirHdr *pDirHdr = (tSndZipDirHdr *)( pDir + dirPos );
			dirPos += pDirHdr->fNameLength + pDirHdr->fExtraLength + pDirHdr->fCommentLength + sizeof( tSndZipDirHdr );

			if ( pDirHdr->fSignature[0] != 'P' || pDirHdr->fSignature[1] != 'K' ||
				 pDirHdr->fSignature[2] != 1 || pDirHdr->fSignature[3] != 2 )
				throw (unsigned char)0;

			if ( !pFile->mSeekSet( pDirHdr->fLocalOffset ) )
				throw (unsigned char)0;
			tSndZipLocalHdr localHdr;
			if ( pFile->mRead( &localHdr, sizeof( localHdr ) ) != sizeof( localHdr ) )
				throw (unsigned char)0;

			unsigned long len = localHdr.fNameLength;
			char *pEntryName = new char[len + 1];
			if ( pEntryName == 0 )
				throw (unsigned char)0;
			if ( pFile->mRead( pEntryName, len ) != len )
				throw (unsigned char)0;
			for ( unsigned long j = 0; j < len; j++ )
			{
				pEntryName[j] = tolower( pEntryName[j] );
				if ( pEntryName[j] == '\\' )
					pEntryName[j] = '/';
			}
			pEntryName[len] = 0;

			field_0[i].field_0 = pEntryName;
			field_0[i].field_4 = pFile->mFilePos() + localHdr.fExtraLength;
			field_0[i].field_10 = pDirHdr->fMethod;
			field_0[i].field_8 = pDirHdr->fCompressedSize;
			field_0[i].field_c = pDirHdr->fSize;
		}

		qsort( field_0, field_4, sizeof( tSndZipEntry ), QsortCompare );
	}
	catch ( unsigned char )
	{
		// HYPOTHESIS: the handler is not scored; it releases what was opened and fails.
		if ( pDir )
			delete pDir;
		if ( pFile )
		{
			pFile->mClose();
			delete pFile;
		}
		mDone();
		return 0;
	}

	if ( pDir )
		delete pDir;
	pFile->mClose();
	delete pFile;
	field_10c = 1;
	return 1;
}
