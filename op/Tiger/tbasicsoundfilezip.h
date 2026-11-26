//	tbasicsoundfilezip.h: tSndZipInfoMgr, the zip directory of the sound files.

#ifndef tbasicsoundfilezip_h
#define tbasicsoundfilezip_h

#include "Tiger/tstring.h"

// HYPOTHESIS: layout from the constructor and mInit; names of the fields are not known.
class tSndZipInfoMgr
{
public:
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

	tSndZipInfoMgr();
	// HYPOTHESIS: out of line in the exe (tSndMgr_Miles' destructor calls it); it runs mDone and
	// destroys field_110 (0x0050cb23 does the same on unwind). Not written yet.
	~tSndZipInfoMgr();

	unsigned char	mInit( const char * );
	void			mDone();

	// HYPOTHESIS: the lookup of a name in the sorted directory (0x0066cec0), as
	// tZipInfoMgr::mFindEntry is for the data zip; its name is not known.
	const tSndZipEntry	*fn_0066cec0( const char * ) const;

protected:
	tSndZipEntry	*field_0;
	long			field_4;
	char			field_8[260];
	long			field_10c;
	tString			field_110[8];
	long			field_250;
};

typedef char tSndZipInfoMgr_size_is_0x254[sizeof( tSndZipInfoMgr ) == 0x254 ? 1 : -1];

#endif
