//	tZipInfoMgr.cpp: tZipInfoMgr, the zip directory of the data files.

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "Tiger/tbasicfile.h"
#include "Tiger/tZipInfoMgr.h"


// FUNCTION: STARFLEETOP 0x00627f50
tZipInfoMgr::tZipInfoMgr()
{
	field_0 = 0;
	field_4 = 0;
	field_10c = 0;
}

// FUNCTION: STARFLEETOP 0x00627f60
void tZipInfoMgr::mDone()
{
	if ( field_0 )
	{
		delete[] field_0;
		field_0 = 0;
	}
	field_4 = 0;
	field_10c = 0;
}

// HYPOTHESIS: the zip records, packed as they are on disk (the end record is copied as 22
// bytes and the directory fields are read at odd offsets).
#pragma pack( push, 1 )
struct tZipEndHdr
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

struct tZipDirHdr
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

struct tZipLocalHdr
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

// HYPOTHESIS: the entry compare function qsort is given (0x00628480).
int		fn_00628480( const void *, const void * );

// HYPOTHESIS: the search for the end of central directory record was a helper the
// compiler inlined (the record is copied twice, once to check it and once for the caller).
inline unsigned char GetDirectoryEndHdr( tBasicFile *pFile, tZipEndHdr *pHdr )
{
	static char kSig[4] = { 'P', 'K', 5, 6 };

	unsigned char ok = 0;
	long size = pFile->mFileSize();
	if ( size >= 22 )
	{
		long n = 0x11a;
		if ( size < n )
			n = size;
		long base = size - n;
		pFile->mSeekEnd( -n );

		char buf[0x11a];
		if ( pFile->mRead( buf, n ) == (unsigned long)n )
		{
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
			if ( p < pEnd )
			{
				tZipEndHdr hdr;
				memcpy( &hdr, p, sizeof( hdr ) );
				if ( hdr.fDisk == 0 && hdr.fDirDisk == 0 && hdr.fNumDiskEntries == hdr.fNumEntries &&
					 hdr.fDirSize <= (unsigned long)( size - 22 ) && hdr.fDirOffset <= (unsigned long)( size - 22 ) )
				{
					long hdrPos = ( p - buf ) + base;
					if ( hdrPos + 22 + hdr.fCommentLength <= size )
					{
						memcpy( pHdr, p, sizeof( *pHdr ) );
						ok = 1;
					}
				}
			}
		}
	}
	return ok;
}

// FUNCTION: STARFLEETOP 0x00627fa0
unsigned char tZipInfoMgr::mInit( const char *pName )
{
	static const char *data_zip = "~{n{4`sj\x1a";

	tBasicFile			*pFile = 0;
	char				name[60];

	try
	{
		if ( pName == 0 )
		{
			const char *src = data_zip;
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

		pFile = tBasicFile::mNewBasicFile();
		if ( pFile == 0 )
			throw (unsigned char)0;
		if ( !pFile->mOpenForRead( pName ) )
			throw (unsigned char)0;
		strcpy( field_8, pName );

		tZipEndHdr endHdr;
		if ( !GetDirectoryEndHdr( pFile, &endHdr ) )
			throw (unsigned char)0;

		field_4 = endHdr.fNumDiskEntries;
		field_0 = new tZipEntry[field_4];
		if ( field_0 == 0 )
			throw (unsigned char)0;

		if ( !pFile->mSeekSet( endHdr.fDirOffset ) )
			throw (unsigned char)0;

		for ( long i = 0; i < field_4; i++ )
		{
			tZipDirHdr dirHdr;
			if ( pFile->mRead( &dirHdr, sizeof( dirHdr ) ) != sizeof( dirHdr ) )
				throw (unsigned char)0;
			long nextPos = pFile->mFilePos() + dirHdr.fNameLength + dirHdr.fExtraLength + dirHdr.fCommentLength;

			if ( dirHdr.fSignature[0] != 'P' || dirHdr.fSignature[1] != 'K' ||
				 dirHdr.fSignature[2] != 1 || dirHdr.fSignature[3] != 2 )
				throw (unsigned char)0;

			if ( !pFile->mSeekSet( dirHdr.fLocalOffset ) )
				throw (unsigned char)0;
			tZipLocalHdr localHdr;
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
			field_0[i].field_10 = dirHdr.fMethod;
			field_0[i].field_8 = dirHdr.fCompressedSize;
			field_0[i].field_c = dirHdr.fSize;

			if ( !pFile->mSeekSet( nextPos ) )
				throw (unsigned char)0;
		}

		qsort( field_0, field_4, sizeof( tZipEntry ), fn_00628480 );
	}
	catch ( unsigned char )
	{
		// HYPOTHESIS: the handler is not scored; it releases the file and fails.
		if ( pFile )
		{
			pFile->mClose();
			delete pFile;
		}
		mDone();
		return 0;
	}

	pFile->mClose();
	delete pFile;
	field_10c = 1;
	return 1;
}
