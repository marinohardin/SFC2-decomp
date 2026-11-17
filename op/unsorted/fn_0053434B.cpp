// Unit:      unsorted/fn_0053434B.cpp
// Functions: 0x0053434b to 0x00534e70 (op/UNITS.tsv); original file unknown.

#include "Tiger/tstring.h"
#include "Tiger/tstringtag.h"

// HYPOTHESIS: owning class unknown; only the long at +0x44 is evidenced.
class fn_0053434b_object
{
public:
    long fn_0053434b() const;

    char field_00[0x44];
    long field_44;
};

// FUNCTION: STARFLEETOP 0x0053434b
long fn_0053434b_object::fn_0053434b() const
{
    return field_44;
}

// HYPOTHESIS: a global tString at 0x00c2f3f8; its name is not known. The compiler generates
// its dynamic initializer (0x00534391, a tail call to the constructor) from this definition.
// FUNCTION: STARFLEETOP 0x0053439b
// FUNCTION: STARFLEETOP 0x00534391
tString g_00c2f3f8;

// HYPOTHESIS: a __thiscall on an object whose first member is a tString and which returns
// itself; the owning class is not known.
class fn_005343b1_object
{
public:
    fn_005343b1_object& fn_005343b1(char* spec, long index);

    tString field_00;
};

// FUNCTION: STARFLEETOP 0x005343b1
fn_005343b1_object& fn_005343b1_object::fn_005343b1(char* spec, long index)
{
    tStringGroupTag* group = new tStringGroupTag(spec);
    field_00 = group->mGetString(index);
    delete group;
    return *this;
}

// HYPOTHESIS: a constructor of an object whose first member is a std::vector (the allocator byte
// copied from an uninitialised default argument, then three zeroed pointers); the owning class
// and the element type are not known.
class fn_0053440f_object
{
public:
    fn_0053440f_object();
    ~fn_0053440f_object();
    // HYPOTHESIS: a member taking no arguments, called first by the destructor.
    void fn_005ff7ac();

    std::vector< long > field_00;
};

// FUNCTION: STARFLEETOP 0x0053440f
fn_0053440f_object::fn_0053440f_object()
{
}

// FUNCTION: STARFLEETOP 0x00534425
fn_0053440f_object::~fn_0053440f_object()
{
    fn_005ff7ac();
}
