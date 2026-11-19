//	Settings.h: tSettings, the profile settings read from a file.
//	Only the members matched code calls are declared; their signatures are the SDK library's
//	symbols (the two numeric GetList overloads at 0x006f7a00 and 0x006f7be0).

#if !defined( Settings_h )
#define Settings_h

#pragma once

#include <string>
#include <vector>

class tSettings
{
public:
	virtual ~tSettings();
	void GetValue( const std::string& name, long* value );
	void GetList( const std::string& name, std::vector<std::string>* list, std::vector<std::string>* defaultList );
	void GetList( const std::string& name, std::vector< long >* values, std::vector< std::string >* strings );
	void GetList( const std::string& name, std::vector< float >* values, std::vector< std::string >* strings );
	void GetValue( const std::string& name, std::string* value );
};

#endif
