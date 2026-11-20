#ifndef TEXBMP_H
#define TEXBMP_H

class tBasicFile;

// HYPOTHESIS: slot order from the exe's vtable for tTexBmp (0x007a16dc); only vfunction2 is
// rebuilt, so the other slots keep placeholder signatures. Field names are unknown; offsets
// and widths come from vfunction2, which fills them from a bitmap file's headers.
class tTexBmp
{
public:
	virtual tBasicFile	*vfunction1( void );				// slot 0: create the file object
	virtual void	vfunction2( tBasicFile *file );			// slot 1: read the headers
	virtual void	vfunction3( tBasicFile *file );			// slot 2: read a PCX file's headers
	virtual void	vfunction4( tBasicFile *file );			// slot 3
	virtual void	vfunction5( tBasicFile *file );			// slot 4: read a PCX file's pixels
	virtual			~tTexBmp();								// slot 5
	// The exe's error string names this method tTexBmp::mInit.
	virtual bool	vfunction7( const char *name );			// slot 6: mInit
	virtual bool	vfunction8( void );						// slot 7: reset, release buffers

	void			fn_006a5c20( void );					// clear the description
	bool			mLock( void );							// read the pixels on first lock
	// HYPOTHESIS: the counterpart of mLock; tSFCTexMgrEx::vfunction4 calls it after mLock.
	void			fn_006a6500( void );

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
