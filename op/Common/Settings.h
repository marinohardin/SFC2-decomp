#ifndef COMMON_SETTINGS_H
#define COMMON_SETTINGS_H

#include <string>
#include <vector>

// Declared only as far as matched code uses it; the signatures are the SDK library symbols
// (GetValue at 0x006f75d0, GetList at 0x006f7830).
class tSettings
{
public:
    void GetValue(const std::string &name, std::string *value);
    void GetList(const std::string &key, std::vector< std::string > *values, std::vector< std::string > *defaults);
};

#endif
