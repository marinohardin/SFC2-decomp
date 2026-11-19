#include "Common/Settings.h"

class tCommonSettings : public tSettings
{
public:
	tCommonSettings();
	virtual ~tCommonSettings();
};

// FUNCTION: STARFLEETOP 0x0040c8d2
tCommonSettings::tCommonSettings()
	: tSettings( std::string( "CommonSettings.gf" ) )
{
}

// FUNCTION: STARFLEETOP 0x0040c904
tCommonSettings::~tCommonSettings()
{
}
