#include "unsorted/tCampaignInfo.h"

// MATCH-ONLY: the destructor's own code (0x0042ceae) is the member and base destructors the
// compiler runs; its recorded boundary stops after 18 bytes, so it is claimed only through the
// deleting destructor, which is exact.
// FUNCTION: STARFLEETOP 0x004617f1
tCampaignInfo::~tCampaignInfo()
{
}

#include <vector>
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
