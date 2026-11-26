#include "unsorted/tCampaignInfo.h"

// MATCH-ONLY: the destructor's own code (0x0042ceae) is the member and base destructors the
// compiler runs; its recorded boundary stops after 18 bytes, so it is claimed only through the
// deleting destructor, which is exact.
// FUNCTION: STARFLEETOP 0x004617f1
tCampaignInfo::~tCampaignInfo()
{
}

#include <vector>
#include <stdlib.h>
#include <string>

void fn_004064d5(std::string* _P, const std::string& _V);

// HYPOTHESIS: the library's vector<std::string> copy constructor, written on a class of the
// vector's layout (allocator byte, first, last, end).
class fn_0046180d_Vector
{
public:
    fn_0046180d_Vector(const fn_0046180d_Vector& _X);
    char _A;
    std::string* _First;
    std::string* _Last;
    std::string* _End;
    // MATCH-ONLY: the library writes this as a conditional expression, which folds into the
    // allocation's clamp; the original keeps the two zero paths apart.
    unsigned int size() const { if (_First == 0) return 0; return _Last - _First; }
};

// MATCH-ONLY: the library's _Allocate helper, which the original inlined.
inline std::string* fn_0046180d_Allocate(int _N, std::string*)
{
    if (_N < 0)
        _N = 0;
    return (std::string*)operator new((unsigned int)_N * sizeof(std::string));
}

// MATCH-ONLY: the library's _Ucopy helper, which the original inlined.
inline std::string* fn_0046180d_Ucopy(std::string* _F, std::string* _L, std::string* _P)
{
    for (; _F != _L; ++_P, ++_F)
        fn_004064d5(_P, *_F);
    return _P;
}

// FUNCTION: STARFLEETOP 0x0046180d
fn_0046180d_Vector::fn_0046180d_Vector(const fn_0046180d_Vector& _X)
{
    _A = _X._A;
    _First = fn_0046180d_Allocate((int)_X.size(), (std::string*)0);
    _Last = fn_0046180d_Ucopy(_X._First, _X._Last, _First);
    _End = _Last;
}

// HYPOTHESIS: an output iterator of the library's uninitialized-copy helper: a polymorphic
// object whose second virtual constructs one element, carrying two words of state.
class fn_00461875_Iterator
{
public:
    virtual void fn_vfunction0();
    virtual void fn_vfunction1();
    virtual void fn_vfunction2(void* _P);
    // HYPOTHESIS: an empty inline destructor; the original's frame unwinds the by-value copy.
    ~fn_00461875_Iterator() {}
    int _A;
    int _B;
};

struct fn_00461875_Element
{
    char _Data[0x16c];
};

// FUNCTION: STARFLEETOP 0x00461875
fn_00461875_Iterator fn_00461875(fn_00461875_Element* _F, fn_00461875_Element* _L, fn_00461875_Iterator _O)
{
    for (; _F != _L; ++_F)
    {
        fn_00461875_Iterator* _P = &_O;
        _P->fn_vfunction2(_F);
    }
    return _O;
}

// HYPOTHESIS: the atexit registration of a global whose destructor wrapper is the
// one-byte function at 0x004bd431.
void fn_004bd431();

// FUNCTION: STARFLEETOP 0x00461903
void fn_00461903()
{
    atexit(fn_004bd431);
}
