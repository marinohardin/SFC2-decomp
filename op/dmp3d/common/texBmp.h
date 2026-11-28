#ifndef TEXBMP_H
#define TEXBMP_H

class tBasicFile;

// Slot order from the exe's vtable for tTexBmp (0x007a16dc); slots 1 to 4, 6 and 7 carry their SDK
// symbols (slot 0 has no single match yet). Field names are unknown; offsets and widths come from
// mInitAsBMP, which fills them from a bitmap file's headers.
class tTexBmp
{
public:
	virtual tBasicFile	*vfunction1( void );				// slot 0: create the file object
protected:
	virtual void	mInitAsBMP( tBasicFile *file );			// slot 1: read the headers
	virtual void	mInitAsPCX( tBasicFile *file );			// slot 2: read a PCX file's headers
	virtual void	mReadBMP( tBasicFile *file );			// slot 3
	virtual void	mReadPCX( tBasicFile *file );			// slot 4: read a PCX file's pixels
public:
	virtual			~tTexBmp();								// slot 5
	virtual unsigned char	mInit( const char *name );		// slot 6
	virtual unsigned char	mDone( void );					// slot 7: reset, release buffers

protected:
	void			mResetDataMembers( void );				// clear the description
public:
	bool			mLock( void );							// read the pixels on first lock
	// The counterpart of mLock; tSFCTexMgrEx::vfunction4 calls it after mLock.
	unsigned char	mUnlock( void );

	long			field_04;		// 1 for a Windows header, 2 for an OS/2 core header
	char			field_08[0x104];
	long			field_10c;		// bits per pixel
	long			field_110;		// width
	long			field_114;		// height, always positive
	long			field_118;
	long			field_11c;
	long			field_120;		// bytes per row, padded to 4
	unsigned char	field_124;		// rows stored bottom-up
	long			field_128;		// image size in bytes
	long			field_12c;
	long			field_130;		// file position of the pixel data
	long			field_134;		// file position of the palette
	long			field_138;		// palette entry count
	long			field_13c;
	long			field_140;
};

#endif
