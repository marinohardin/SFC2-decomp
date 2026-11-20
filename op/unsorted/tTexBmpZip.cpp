#include "dmp3d/common/texBmp.h"
#include "Tiger/tbasicfile.h"
#include "Tiger/tZipInfoMgr.h"

tBasicFile *_NewBasicFileZip( tZipInfoMgr *zipInfoMgr );

// HYPOTHESIS: tTexBmpZip derives from tTexBmp and keeps the zip directory right after it.
class tTexBmpZip : public tTexBmp
{
public:
	virtual tBasicFile	*vfunction1( void );

	tZipInfoMgr		*field_144;
};

// FUNCTION: STARFLEETOP 0x0040c7e7
tBasicFile *tTexBmpZip::vfunction1( void )
{
	tZipInfoMgr *zip = field_144;
	// HYPOTHESIS: field_10c of tZipInfoMgr is protected; read through its offset.
	if ( zip == 0 || *(long *)((char *)zip + 0x10c) != 1 )
		return tBasicFile::mNewBasicFile();
	return _NewBasicFileZip( zip );
}
