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
    void fn_00534454(void* value);
    void fn_00571a69(void* const& value);

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

// HYPOTHESIS: appends its argument to the vector at +0 through 0x00571a69 (that vector type is
// declared only inside tPanelMgr.cpp, so the append is declared on this object); 0x0053455b
// calls it on one of the slots of fn_005344a3_object, so it is a member of the slot class.

// FUNCTION: STARFLEETOP 0x00534454
void fn_0053440f_object::fn_00534454(void* value)
{
    fn_00571a69(value);
}

int fn_0040f9ea(char *a, char *b);

// HYPOTHESIS: an element whose name string sits at +4; its class is not known.
class fn_00534461_entry
{
public:
    void* field_00;
    char* field_04;
};

// HYPOTHESIS: an object whose first member is a vector of entry pointers; this searches it from
// the back for the entry of the given name (0x0040f9ea compares two strings, 0 when equal) and
// returns its index, or -1.
class fn_00534461_object
{
public:
    int fn_00534461(char* name);

    std::vector< fn_00534461_entry* > field_00;
};

// FUNCTION: STARFLEETOP 0x00534461
int fn_00534461_object::fn_00534461(char* name)
{
    int i = field_00.size();
    while (i-- != 0)
    {
        if (fn_0040f9ea(field_00[i]->field_04, name) == 0)
            return i;
    }
    return -1;
}

// HYPOTHESIS: a constructor of an object holding a tString, the long it is given and twenty
// fn_0053440f_object pointers, filled from the last; the owning class is not known.
class fn_005344a3_object
{
public:
    fn_005344a3_object(long value);
    ~fn_005344a3_object();
    // HYPOTHESIS: the argument's first long selects the slot it is handed to.
    void fn_0053455b(long* value);

    tString field_00;
    long field_28;
    fn_0053440f_object* field_2c[20];
};

// FUNCTION: STARFLEETOP 0x005344a3
fn_005344a3_object::fn_005344a3_object(long value)
{
    field_28 = value;
    unsigned int n = 20;
    while (n-- > 0)
        field_2c[n] = new fn_0053440f_object();
}

// FUNCTION: STARFLEETOP 0x005344ff
fn_005344a3_object::~fn_005344a3_object()
{
    unsigned int n = 20;
    while (n-- > 0)
        delete field_2c[n];
}

// FUNCTION: STARFLEETOP 0x0053455b
void fn_005344a3_object::fn_0053455b(long* value)
{
    field_2c[*value]->fn_00534454(value);
}
