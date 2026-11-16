//	tbasicfile.h: tBasicFile, the file interface Tiger reads and writes files through.
//
//	Member names are the SDK library's (its file classes override them); the slot order is
//	the exe's vtable of tSoundFileIO (0x007b794c) and agrees with the call offsets in every
//	unit that uses the interface.

#ifndef tbasicfile_h
#define tbasicfile_h

class tBasicFile
{
public:
	// HYPOTHESIS: the destructor is inline and empty; derived destructors end with the
	// base vtable store only (tSoundFileIO's, 0x0066f190).
	virtual					~tBasicFile( void ) {}
	virtual unsigned char	mOpenForRead( const char *name );			// +0x04
	virtual unsigned char	mOpenForWrite( const char *name );			// +0x08
	virtual unsigned char	mIsOpen( void );							// +0x0c
	virtual unsigned char	mClose( void );								// +0x10
	virtual unsigned char	mEOF( void );								// +0x14
	virtual unsigned long	mRead( void *buffer, unsigned long size );	// +0x18
	virtual unsigned long	mWrite( const void *buffer, unsigned long size );	// +0x1c
	virtual unsigned long	mFilePos( void );							// +0x20
	virtual unsigned long	mFileSize( void );							// +0x24
	virtual unsigned char	mSeekSet( long position );					// +0x28
	virtual unsigned char	mSeekCur( long offset );					// +0x2c
	virtual unsigned char	mSeekEnd( long offset );					// +0x30

	// HYPOTHESIS: an inline wrapper; the exe keeps an out-of-line copy at 0x00520990.
	tBasicFile				*mDispose( void )
	{
		delete this;
		return 0;
	}

	// HYPOTHESIS: the static that releases the basic file layer; the analysis matches
	// 0x00627130 to the SDK library's tBasicFile::mDone.
	static void				mDone( void );

	// Name from the SDK library symbol matched to 0x00627140.
	static tBasicFile		*mNewBasicFile( void );
};

#endif	// tbasicfile_h
