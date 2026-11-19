#ifndef TFIGHTERSPEC_H
#define TFIGHTERSPEC_H

#include <string>

// HYPOTHESIS: the 16-byte element of tFighterSpec's array at +0x68; its constructor is the
// out-of-line copy at 0x0040195f, its destructor an empty function.
struct fn_00401862_entry
{
    fn_00401862_entry();
    ~fn_00401862_entry();

    long m00;
    long m04;
    long m08;
    char m0c;
};

inline fn_00401862_entry::fn_00401862_entry()
{
}

inline fn_00401862_entry::~fn_00401862_entry()
{
}

// HYPOTHESIS: layout from the constructor at 0x00401862 (vtable 0x00795644).
class tFighterSpec
{
public:
    tFighterSpec();
    tFighterSpec(const tFighterSpec&);
    virtual ~tFighterSpec();

    long m04;
    std::string m08;
    long m18;
    long m1c;
    long m20;
    long m24;
    long m28;
    long m2c;
    long m30;
    long m34;
    long m38;
    long m3c;
    long m40;
    long m44;
    std::string m48;
    std::string m58;
    fn_00401862_entry m68[5];
};

#endif
