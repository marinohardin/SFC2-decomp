//	tZipInfoMgr.h: tZipInfoMgr, the zip directory of the data files.

#ifndef TZIPINFOMGR_H
#define TZIPINFOMGR_H

// HYPOTHESIS: layout from the constructor, by analogy with tSndZipInfoMgr; the names of the
// fields are not known.
namespace tBasicFileZipNamespace
{
	class tBasicFileZip;
}

class tZipInfoMgr
{
	// HYPOTHESIS: the zip file reads the entries and the archive name directly.
	friend class tBasicFileZipNamespace::tBasicFileZip;

public:
	tZipInfoMgr();

	unsigned char	mInit( const char * );
	void			mDone();

protected:
	class tZipEntry;

public:
	// The lookup of a name in the sorted directory; name and signature from the exe's
	// symbol at 0x006284c0.
	const tZipEntry	*mFindEntry( const char * ) const;

protected:
	class tZipEntry
	{
		friend class tZipInfoMgr;

	protected:
		tZipEntry();
		~tZipEntry();

	public:
		char			*field_0;
		unsigned long	field_4;
		unsigned long	field_8;
		unsigned long	field_c;
		unsigned long	field_10;
	};

	tZipEntry		*field_0;
	long			field_4;
	char			field_8[260];
	long			field_10c;
};

#endif
