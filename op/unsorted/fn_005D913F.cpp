#include <windows.h>
#include <string>

// HYPOTHESIS: 0x0046de63 is a non-virtual member called on a held pointer (a release
// or close); its class is not known yet.
class fn_0046de63_object
{
public:
    void FUN_0046de63();

    long m00;
    long m04; // HYPOTHESIS: a reference count, raised with InterlockedIncrement on copy
};

// HYPOTHESIS: an object holding a std::string at +0x00 (its destructor is the inlined
// _Tidy(true) at the end) and a pointer at +0x10.
class fn_005d913f_object
{
public:
    fn_005d913f_object(const fn_005d913f_object &other);
    ~fn_005d913f_object();

    std::string m00;
    fn_0046de63_object *m10;
};

// FUNCTION: STARFLEETOP 0x005d913f
fn_005d913f_object::~fn_005d913f_object()
{
    if (m10)
        m10->FUN_0046de63();
}

// FUNCTION: STARFLEETOP 0x005d917a
fn_005d913f_object::fn_005d913f_object(const fn_005d913f_object &other)
    : m00(other.m00)
{
    fn_0046de63_object *p = other.m10;
    m10 = p ? (::InterlockedIncrement(&p->m04), p) : 0;
}

// HYPOTHESIS: a counted pointer to an fn_0046de63_object held at +0x00; this assigns a
// raw pointer, taking a reference on the new one and releasing the old.
class fn_005d91cc_object
{
public:
    fn_0046de63_object *operator=(fn_0046de63_object *p);

    fn_0046de63_object *m00;
};

// FUNCTION: STARFLEETOP 0x005d91cc
fn_0046de63_object *fn_005d91cc_object::operator=(fn_0046de63_object *p)
{
    if (m00 != p)
    {
        if (m00)
            m00->FUN_0046de63();
        m00 = p ? (::InterlockedIncrement(&p->m04), p) : 0;
    }
    return p;
}
