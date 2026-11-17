//	Settings.h: tSettings, the profile settings read from a file.
//	Only the members matched code calls are declared; their signatures are the SDK library's
//	symbols.

#if !defined( Settings_h )
#define Settings_h

#pragma once

#include <string>
#include <vector>

class tSettings
{
public:
	void GetList( const std::string& name, std::vector<std::string>* list, std::vector<std::string>* defaultList );
	void GetValue( const std::string& name, std::string* value );
};

#endif
