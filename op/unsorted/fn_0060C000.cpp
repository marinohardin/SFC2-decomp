// Functions of page 0x0060c000 with no unit or class evidence; original file unknown.

#include <memory>

// HYPOTHESIS: the auto_ptr's pointee; the out-of-line ~auto_ptr at 0x00607364 deletes it
// through vtable slot 0, so it has a virtual destructor. Its name is unknown.
class C_00c3a7c0
{
public:
    virtual ~C_00c3a7c0();
};

// HYPOTHESIS: a static auto_ptr at 0x00c3a7c0 (the bool _Owns, then the pointer); name unknown.
// The compiler's code for it: the initializer pair at 0x0060c14f (call, jmp), the inline
// constructor expanded at 0x0060c159, the atexit registration at 0x0060c168 and the atexit
// destructor at 0x0060c174.
// FUNCTION: STARFLEETOP 0x0060c168
// FUNCTION: STARFLEETOP 0x0060c159
std::auto_ptr<C_00c3a7c0> g_00c3a7c0;
