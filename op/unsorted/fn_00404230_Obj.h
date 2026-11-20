//	fn_00404230_Obj.h: the stream buffer object whose members unsorted/fn_00404000.cpp holds; the
//	ifstream constructor in unsorted/fn_00403000.cpp reaches its open (0x00404230).
// Evidence:  calls (original name and file unknown)

#if !defined( fn_00404230_Obj_h )
#define fn_00404230_Obj_h

#include <string>
#include <locale>

// HYPOTHESIS: unknown class; m50 is a state word tested before the lookup.
class fn_00404230_Item;

class fn_00404230_Obj
{
public:
    fn_00404230_Obj* fn_00404230(unsigned long a, unsigned long b);
    void fn_0040499a(fn_00404230_Item* item, int n);
    void fn_00404a0c();
    fn_00404230_Obj* fn_00404267();
    std::locale getloc() const;
    char pad0[0x38];
    std::codecvt<char, char, int>* m38;
    char pad3c[0x08];
    std::string* m44;
    char pad48[0x04];
    std::locale m4c;
    unsigned long m50;
};

#endif
