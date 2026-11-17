// Unit:      unsorted/fn_0053434B.cpp
// Functions: 0x0053434b to 0x00534e70 (op/UNITS.tsv); original file unknown.

#include "Tiger/tstring.h"
#include "Tiger/tstringtag.h"
#include "unsorted/fn_0090be9c_type.h"
#include "unsorted/tMissionCompleteResults.h"
#include <stdlib.h>
#include <string.h>

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

// HYPOTHESIS: an element whose name string sits at +4; its class is not known.
class fn_00534461_entry
{
public:
    void* field_00;
    char* field_04;
};

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
    int fn_00534461(char* name);

    std::vector< fn_00534461_entry* > field_00;
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

// HYPOTHESIS: searches the vector from the back for the entry of the given name (0x0040f9ea
// compares two strings, 0 when equal) and returns its index, or -1. It is a member of the slot
// class because 0x0053456e calls it on the slots of fn_005344a3_object.

// FUNCTION: STARFLEETOP 0x00534461
int fn_0053440f_object::fn_00534461(char* name)
{
    int i = field_00.size();
    while (i-- != 0)
    {
        if (fn_0040f9ea(field_00[i]->field_04, name) == 0)
            return i;
    }
    return -1;
}

// HYPOTHESIS: a 12-byte entry whose long at +4 indexes the "RaceRanks" strings; the rest is
// not known.
class fn_0053462c_entry
{
public:
    long field_00;
    long field_04;
    long field_08;
};

// HYPOTHESIS: a constructor of an object holding a tString, the long it is given and twenty
// fn_0053440f_object pointers, filled from the last; the owning class is not known.
class fn_005344a3_object
{
public:
    fn_005344a3_object(long value);
    ~fn_005344a3_object();
    // HYPOTHESIS: the argument's first long selects the slot it is handed to.
    void fn_0053455b(long* value);
    int fn_0053456e(char* name);
    void fn_00534622(fn_0053462c_entry* value);
    tString& fn_0053462c(long index);
    tString& fn_00534647(long index);
    long fn_00534690(long index);
    long fn_005346a0(long index);
    long fn_005346d9(long value);

    tString field_00;
    long field_28;
    fn_0053440f_object* field_2c[20];
    // HYPOTHESIS: an array of 12-byte entries directly after the slots, set by 0x00534622 and
    // indexed by 0x0053462c.
    fn_0053462c_entry* field_7c;
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

// HYPOTHESIS: returns the slot holding an entry of the given name; when field_28 is 16 it
// tries the name with a "2" appended as well. Returns -1 when no slot has it.
// FUNCTION: STARFLEETOP 0x0053456e
int fn_005344a3_object::fn_0053456e(char* name)
{
    unsigned int n = 20;
    while (n != 0)
    {
        n--;
        if (field_2c[n]->fn_00534461(name) != -1)
            return n;
    }
    if (field_28 == 16)
    {
        tString alt(name);
        alt.mAppendInt32s(2);
        n = 20;
        while (n != 0)
        {
            n--;
            if (field_2c[n]->fn_00534461(alt.mCharStr()) != -1)
                return n;
        }
    }
    return -1;
}

// HYPOTHESIS: a setter on the same object (it follows its members in address order).
// FUNCTION: STARFLEETOP 0x00534622
void fn_005344a3_object::fn_00534622(fn_0053462c_entry* value)
{
    field_7c = value;
}

// HYPOTHESIS: looks up the string of the given entry's rank in the "RaceRanks" group; the
// callee runs on this same object (both start with their tString), so the two classes are
// probably one class not yet joined.
// FUNCTION: STARFLEETOP 0x0053462c
tString& fn_005344a3_object::fn_0053462c(long index)
{
    return ((fn_005343b1_object*)this)->fn_005343b1("RaceRanks", field_7c[index].field_04).field_00;
}

// HYPOTHESIS: the entries end with field_00 0x7fffffff; the first entry with field_00 -1
// starts the run the index counts from. Without one it returns the empty global string.
// FUNCTION: STARFLEETOP 0x00534647
tString& fn_005344a3_object::fn_00534647(long index)
{
    for (int i = 0; field_7c[i].field_00 != 0x7fffffff; i++)
    {
        if (field_7c[i].field_00 == -1)
            return ((fn_005343b1_object*)this)->fn_005343b1("RaceRanks", field_7c[index + i].field_04).field_00;
    }
    return g_00c2f3f8;
}

// FUNCTION: STARFLEETOP 0x00534690
long fn_005344a3_object::fn_00534690(long index)
{
    return field_7c[index].field_00;
}

// HYPOTHESIS: as 0x00534647, but returns the entry's field_00 (0 when no -1 entry is found).
// FUNCTION: STARFLEETOP 0x005346a0
long fn_005344a3_object::fn_005346a0(long index)
{
    for (int i = 0; field_7c[i].field_00 != 0x7fffffff; i++)
    {
        if (field_7c[i].field_00 == -1)
            return field_7c[index + i].field_00;
    }
    return 0;
}

// HYPOTHESIS: counts the entries after the first whose field_00 does not exceed the value,
// stopping at a field_00 of -1.
// FUNCTION: STARFLEETOP 0x005346d9
long fn_005344a3_object::fn_005346d9(long value)
{
    int i;
    for (i = 1; field_7c[i].field_00 != -1 && !(value < field_7c[i].field_00); i++)
        ;
    return i - 1;
}

// HYPOTHESIS: a constructor of an object holding a tString, the long it is given and two
// std::vectors (element types not known; taken as fn_0053440f_object's, since the destructor
// treats them alike), which it then hands the long to 0x005347bd and
// 0x005347f4; the owning class is not known.
class fn_005346f9_object
{
public:
    fn_005346f9_object(long value);
    ~fn_005346f9_object();
    void fn_005347bd(long value);
    void fn_005347f4(long value);
    void fn_0053482b(bool reset);
    void fn_00534840(bool reset);
    void fn_00534855(std::vector< fn_00534461_entry* >* list, bool reset, char* group);
    // HYPOTHESIS: takes no arguments; 0x00534855 calls it on this object before a pick.
    void fn_00534903();

    tString field_00;
    long field_28;
    std::vector< fn_00534461_entry* > field_2c;
    std::vector< fn_00534461_entry* > field_3c;
    // HYPOTHESIS: 0x00534950 allocates 0x54 bytes for this object; the last eight are not
    // touched by the functions known so far.
    long field_4c;
    long field_50;
};

// FUNCTION: STARFLEETOP 0x005346f9
fn_005346f9_object::fn_005346f9_object(long value)
{
    field_28 = value;
    fn_005347bd(value);
    fn_005347f4(value);
}

// HYPOTHESIS: the destructor calls 0x005ff7ac on both vectors before they are destroyed. That
// call is declared only on fn_0053440f_object (whose only member is such a vector), hence the
// casts; the vectors' destructor is 0x00571a7a, as for fn_0053440f_object's vector.
// FUNCTION: STARFLEETOP 0x0053475f
fn_005346f9_object::~fn_005346f9_object()
{
    ((fn_0053440f_object*)&field_2c)->fn_005ff7ac();
    ((fn_0053440f_object*)&field_3c)->fn_005ff7ac();
}

// HYPOTHESIS: a table of twenty 12-byte entries at 0x0085a298 whose first long is matched
// against the value; the address of the rest of each entry is appended to the first vector.
struct fn_005347bd_entry
{
    long field_00;
    fn_00534461_entry field_04;
};

extern fn_005347bd_entry g_0085a298[20];

// FUNCTION: STARFLEETOP 0x005347bd
void fn_005346f9_object::fn_005347bd(long value)
{
    fn_005347bd_entry* entry = g_0085a298;
    for (int n = 20; n != 0; n--, entry++)
    {
        if (entry->field_00 == value)
            field_2c.push_back(&entry->field_04);
    }
}

// HYPOTHESIS: a second table of twenty entries at 0x0085a388, right after the first, whose
// matches are appended to the second vector.
extern fn_005347bd_entry g_0085a388[20];

// FUNCTION: STARFLEETOP 0x005347f4
void fn_005346f9_object::fn_005347f4(long value)
{
    fn_005347bd_entry* entry = g_0085a388;
    for (int n = 20; n != 0; n--, entry++)
    {
        if (entry->field_00 == value)
            field_3c.push_back(&entry->field_04);
    }
}

// HYPOTHESIS: looks the entry up in the first vector under the "SunNames" string group; the
// callee runs on this same object (ecx is passed through), its return type is not known.
// FUNCTION: STARFLEETOP 0x0053482b
void fn_005346f9_object::fn_0053482b(bool reset)
{
    fn_00534855(&field_2c, reset, "SunNames");
}

// HYPOTHESIS: as 0x0053482b, for the second vector under the "PlanetNames" string group.
// FUNCTION: STARFLEETOP 0x00534840
void fn_005346f9_object::fn_00534840(bool reset)
{
    fn_00534855(&field_3c, reset, "PlanetNames");
}

// HYPOTHESIS: the entries of these two vectors hold a string index at +0 and a byte at +4
// that marks a name as taken.
struct fn_00534855_entry
{
    long field_00;
    char field_04;
};

// HYPOTHESIS: picks a random entry of the list (from the global at 0x0090be9c), steps on to
// the first entry not yet taken (clearing the start one when all are taken), marks it taken and
// loads its string from the given group. The flag first calls 0x00534903.
// FUNCTION: STARFLEETOP 0x00534855
void fn_005346f9_object::fn_00534855(std::vector< fn_00534461_entry* >* list, bool reset, char* group)
{
    if (reset)
        fn_00534903();
    int count = list->size();
    fn_0090be9c_type* source = g_0090be9c;
    int start = (int)((double)source->vfunction7() * count / (double)source->field_20);
    int i = start;
    while (((fn_00534855_entry*)(*list)[i])->field_04)
    {
        i++;
        if (i >= count)
            i = 0;
        if (i == start)
            ((fn_00534855_entry*)(*list)[i])->field_04 = 0;
    }
    ((fn_00534855_entry*)(*list)[i])->field_04 = 1;
    ((fn_005343b1_object*)this)->fn_005343b1(group, ((fn_00534855_entry*)(*list)[i])->field_00);
}

// HYPOTHESIS: clears the taken mark of every entry in both vectors, last to first.
// FUNCTION: STARFLEETOP 0x00534903
void fn_005346f9_object::fn_00534903()
{
    int i = field_2c.size();
    while (i--)
        ((fn_00534855_entry*)field_2c[i])->field_04 = 0;
    i = field_3c.size();
    while (i--)
        ((fn_00534855_entry*)field_3c[i])->field_04 = 0;
}

// HYPOTHESIS: a 12-byte entry whose constructor is 0x00534ac1; the owner's destructor treats
// the entries as rows of 200 ended by a field_00 of -1 and frees the block at field_04.
class fn_00534ac1_entry
{
public:
    fn_00534ac1_entry();

    long field_00;
    void* field_04;
    // HYPOTHESIS: a float; the constructor stores 1.0f (0x3f800000) here.
    float field_08;
};

// HYPOTHESIS: an object holding a tString, 4800 12-byte entries and the owned
// fn_005344a3_object and fn_005346f9_object instances, each given its own index; the owning
// class is not known.
class fn_00534950_object
{
public:
    fn_00534950_object();
    ~fn_00534950_object();
    void fn_00534ad0();
    void fn_00534b64();
    void fn_00534d2a(int kind, fn_00534ac1_entry* row, char* table);
    int fn_00534d66(char* name);
    tString& fn_00534d93(int kind, long index);
    tString& fn_00534db5(int kind, long index);
    long fn_00534dde(int kind, long index);
    long fn_00534dfd(int kind, long index);
    long fn_00534e23(int kind, long value);
    void fn_00534e42(int kind, bool reset);
    void fn_00534e59(int kind, bool reset);

    tString field_00;
    fn_00534ac1_entry field_28[4800];
    fn_005344a3_object* field_e128[25];
    fn_005346f9_object* field_e18c[10];
};

// FUNCTION: STARFLEETOP 0x00534950
fn_00534950_object::fn_00534950_object()
{
    int n = 25;
    while (n--)
        field_e128[n] = new fn_005344a3_object(n);
    n = 10;
    while (n--)
        field_e18c[n] = new fn_005346f9_object(n);
}

// FUNCTION: STARFLEETOP 0x00534a06
fn_00534950_object::~fn_00534950_object()
{
    int j = 0;
    int i = 0;
    while (i < 4800)
    {
        if (field_28[i + j].field_00 == -1 || !field_28[i + j].field_04)
        {
            j = 0;
            i += 200;
        }
        else
        {
            delete field_28[i + j].field_04;
            field_28[i + j].field_04 = 0;
            j++;
        }
    }
    unsigned int n = 25;
    while (n-- > 0)
        delete field_e128[n];
    n = 10;
    while (n-- > 0)
        delete field_e18c[n];
}

// FUNCTION: STARFLEETOP 0x00534ac1
fn_00534ac1_entry::fn_00534ac1_entry()
{
    field_00 = -1;
    field_04 = 0;
    field_08 = 1.0f;
}

// HYPOTHESIS: a record of the global list at 0x008949a0: the row it belongs to at +0x0c, a
// name at +0x14 and a long at +0x34; the rest is not known.
struct fn_0040a699_record
{
    char field_00[0x0c];
    long field_0c;
    char field_10[4];
    char field_14[0x20];
    long field_34;
};

// HYPOTHESIS: the global at 0x008949a0 is of the type whose count (0x005e8f21) tBattles reads.
extern tObject_005b69d0 g_008949a0;

// HYPOTHESIS: fills the entries row by row from the records of the global list: each record's
// long and a copy of its name go to the next free column of its row.
// FUNCTION: STARFLEETOP 0x00534ad0
void fn_00534950_object::fn_00534ad0()
{
    int i;
    int used[24];
    for (i = 0; i < 24; i++)
        used[i] = 0;
    for (i = 0; i < g_008949a0.fn_005e8f21(); i++)
    {
        fn_0040a699_record* record = g_008949a0.fn_0040a699(i);
        if (!record)
            return;
        int row = record->field_0c;
        int column = used[row];
        field_28[row * 200 + column].field_00 = record->field_34;
        field_28[row * 200 + column].field_04 = malloc(strlen(record->field_14) + 1);
        strcpy((char*)field_28[row * 200 + column].field_04, record->field_14);
        used[row] = column + 1;
    }
}

// HYPOTHESIS: tables of unknown type the rows are filled from; 0x00859ec0 and 0x0085a138 serve
// several rows.
extern char g_00859ec0[];
extern char g_00859f20[];
extern char g_00859f80[];
extern char g_00859ff0[];
extern char g_0085a060[];
extern char g_0085a0d8[];
extern char g_0085a198[];
extern char g_0085a1c8[];
extern char g_0085a1b0[];
extern char g_0085a1e0[];
extern char g_0085a138[];

// HYPOTHESIS: the first argument is a kind that does not always equal the row it fills: kinds
// 8..15 fill rows 9..16, kind 16 fills row 8, kinds 17, 18, 23 and 24 fill rows 22, 17, 21
// and 23, and kinds 21 and 23 both fill row 21.
// FUNCTION: STARFLEETOP 0x00534b64
void fn_00534950_object::fn_00534b64()
{
    fn_00534d2a(0, &field_28[0 * 200], g_00859ec0);
    fn_00534d2a(1, &field_28[1 * 200], g_00859f20);
    fn_00534d2a(2, &field_28[2 * 200], g_00859f80);
    fn_00534d2a(3, &field_28[3 * 200], g_00859ff0);
    fn_00534d2a(5, &field_28[5 * 200], g_0085a060);
    fn_00534d2a(4, &field_28[4 * 200], g_0085a0d8);
    fn_00534d2a(16, &field_28[8 * 200], g_0085a198);
    fn_00534d2a(23, &field_28[21 * 200], g_0085a1c8);
    fn_00534d2a(17, &field_28[22 * 200], g_0085a1b0);
    fn_00534d2a(24, &field_28[23 * 200], g_0085a1e0);
    fn_00534d2a(8, &field_28[9 * 200], g_0085a138);
    fn_00534d2a(9, &field_28[10 * 200], g_0085a138);
    fn_00534d2a(10, &field_28[11 * 200], g_0085a138);
    fn_00534d2a(11, &field_28[12 * 200], g_0085a138);
    fn_00534d2a(12, &field_28[13 * 200], g_0085a138);
    fn_00534d2a(13, &field_28[14 * 200], g_0085a138);
    fn_00534d2a(14, &field_28[15 * 200], g_0085a138);
    fn_00534d2a(15, &field_28[16 * 200], g_0085a138);
    fn_00534d2a(18, &field_28[17 * 200], g_00859ec0);
    fn_00534d2a(6, &field_28[6 * 200], g_00859ec0);
    fn_00534d2a(7, &field_28[7 * 200], g_00859ec0);
    fn_00534d2a(19, &field_28[18 * 200], g_00859ec0);
    fn_00534d2a(20, &field_28[19 * 200], g_00859ec0);
    fn_00534d2a(21, &field_28[21 * 200], g_00859ec0);
}

// HYPOTHESIS: hands the table to the kind's fn_005344a3_object, then each entry of the row up
// to the first with field_00 -1; the table is taken to be of 12-byte entries.
// FUNCTION: STARFLEETOP 0x00534d2a
void fn_00534950_object::fn_00534d2a(int kind, fn_00534ac1_entry* row, char* table)
{
    if (kind >= 0)
    {
        field_e128[kind]->fn_00534622((fn_0053462c_entry*)table);
        for (; row->field_00 != -1; row++)
            field_e128[kind]->fn_0053455b(&row->field_00);
    }
}

// HYPOTHESIS: looks the name up in each kind's fn_005344a3_object, last kind first, and
// returns the first result that is not -1.
// FUNCTION: STARFLEETOP 0x00534d66
int fn_00534950_object::fn_00534d66(char* name)
{
    int n = 25;
    while (n--)
    {
        int result = field_e128[n]->fn_0053456e(name);
        if (result != -1)
            return result;
    }
    return -1;
}

// HYPOTHESIS: the rank string of the given kind's entry, or the empty global string for a
// negative kind.
// FUNCTION: STARFLEETOP 0x00534d93
tString& fn_00534950_object::fn_00534d93(int kind, long index)
{
    if (kind >= 0)
        return field_e128[kind]->fn_0053462c(index);
    return g_00c2f3f8;
}

// FUNCTION: STARFLEETOP 0x00534db5
tString& fn_00534950_object::fn_00534db5(int kind, long index)
{
    if (kind >= 0 && index > 0)
        return field_e128[kind]->fn_00534647(index);
    return g_00c2f3f8;
}

// FUNCTION: STARFLEETOP 0x00534dde
long fn_00534950_object::fn_00534dde(int kind, long index)
{
    if (kind >= 0)
        return field_e128[kind]->fn_00534690(index);
    return 0;
}

// FUNCTION: STARFLEETOP 0x00534dfd
long fn_00534950_object::fn_00534dfd(int kind, long index)
{
    if (kind >= 0 && index > 0)
        return field_e128[kind]->fn_005346a0(index);
    return 0;
}

// FUNCTION: STARFLEETOP 0x00534e23
long fn_00534950_object::fn_00534e23(int kind, long value)
{
    if (kind >= 0)
        return field_e128[kind]->fn_005346d9(value);
    return 0;
}

// FUNCTION: STARFLEETOP 0x00534e42
void fn_00534950_object::fn_00534e42(int kind, bool reset)
{
    field_e18c[kind]->fn_0053482b(reset);
}

// FUNCTION: STARFLEETOP 0x00534e59
void fn_00534950_object::fn_00534e59(int kind, bool reset)
{
    field_e18c[kind]->fn_00534840(reset);
}

// HYPOTHESIS: owning class unknown; only the pointer at +8 is evidenced. The call to
// 0x0043463f pushes three arguments, leaves ecx as it is and does not clean the stack, so it
// is taken to be a member of the same class called on this.
class fn_00534e70_object
{
public:
    void fn_0043463f(void* target, long flag, long value);
    void fn_00534e70(long value);

    char field_00[8];
    void* field_08;
};

// FUNCTION: STARFLEETOP 0x00534e70
void fn_00534e70_object::fn_00534e70(long value)
{
    fn_0043463f(field_08, 1, value);
}
