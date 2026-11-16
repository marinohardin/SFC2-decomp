//	tsound_fileio.h: tSoundFileIOBase, the sound file interface, and its factory.

#ifndef tsound_fileio_h
#define tsound_fileio_h

#include "Tiger/tbasic.h"
#include "Tiger/tbasicfile.h"

// HYPOTHESIS: the base classes as tbasicsoundfilezip.cpp names them; the destructor of
// tSoundFileIO (0x0066f190) ends by storing one base vtable (0x007aaef4), and which of the
// two it belongs to is not yet evidenced. The slots follow the exe's vtable of tSoundFileIO
// (0x007b794c). The compiler groups overloaded virtuals that one class introduces, yet the
// two mRead and the two mReadAsync sit apart in that vtable; so they are taken to be
// introduced by different classes: the plain file operations (+0x00 to +0x30) by
// tBasicFile, the sound reads (+0x34 to +0x3c) by tSoundFileIOBase, and the second
// mReadAsync (+0x40) by tSoundFileIO.
class	tSoundFileIOBase : public tBasicFile
{
	public:
		virtual	iTruth	mReadAsync		( void *buffer, int32u size,
										  void (*callback)( void *, int32u ), void *data );
		virtual	int32u	mRead			( void *buffer, int32u size, int32u position );
		// HYPOTHESIS: +0x3c reports whether an asynchronous read is in progress (mRead
		// refuses to read when it is); it is a stub shared with other classes in
		// tSoundFileIO's vtable, and its name is not known.
		virtual	iTruth	vfunc_03c		( void );
};

tSoundFileIOBase	*_NewSoundFileIO	( void );
int8u				_InitSoundFileIO	( void );

#endif	// tsound_fileio_h
