//	tbasicfilezip.h: tBasicFileZip, the file interface over a file stored in the data zip.
//
//	Member names are the SDK library's (symbols matched to 0x006285b0..0x006288c0).

#ifndef tbasicfilezip_h
#define tbasicfilezip_h

#include "Tiger/tbasicfile.h"

class tZipInfoMgr;

namespace tBasicFileZipNamespace
{

// HYPOTHESIS: layout from mOpenForRead; the names of the fields are not known.
class tBasicFileZip : public tBasicFile
{
public:
	// HYPOTHESIS: an inline constructor, seen only inlined in _NewBasicFileZip.
	tBasicFileZip( tZipInfoMgr *zipInfoMgr )
	{
		field_4 = tBasicFile::mNewBasicFile();
		field_18 = zipInfoMgr;
		field_8 = 0;
		field_c = 0;
		field_10 = 0;
		field_14 = 0;
	}

	virtual unsigned char	mOpenForRead( const char *name );
	virtual unsigned char	mOpenForWrite( const char *name );
	virtual unsigned char	mIsOpen( void );
	virtual unsigned char	mClose( void );
	virtual unsigned char	mEOF( void );
	virtual unsigned long	mRead( void *buffer, unsigned long size );
	virtual unsigned long	mWrite( const void *buffer, unsigned long size );
	virtual unsigned long	mFilePos( void );
	virtual unsigned long	mFileSize( void );
	virtual unsigned char	mSeekSet( long position );
	virtual unsigned char	mSeekCur( long offset );
	virtual unsigned char	mSeekEnd( long offset );

protected:
	virtual					~tBasicFileZip( void );

	tBasicFile		*field_4;
	unsigned long	field_8;
	unsigned long	field_c;
	unsigned long	field_10;
	unsigned char	field_14;
	tZipInfoMgr		*field_18;
};

}

#endif	// tbasicfilezip_h
