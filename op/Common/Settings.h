// Common/Settings.h
//
// Only what the rebuilt functions need is declared yet; the two GetList overloads are the
// SDK library's symbols at 0x006f7a00 and 0x006f7be0.

#if !defined( Settings_h )
#define Settings_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

class tSettings
{
public:
	void GetList( const std::string& name, std::vector< long >* values, std::vector< std::string >* strings );
	void GetList( const std::string& name, std::vector< float >* values, std::vector< std::string >* strings );
};

#endif
