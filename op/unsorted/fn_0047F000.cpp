#include <string>

// HYPOTHESIS: free function (hidden return slot, one stack argument); the owner class is unknown.
// FUNCTION: STARFLEETOP 0x0047f05d
std::string __stdcall fn_0047f05d(int index)
{
    std::string s;
    switch (index) {
    case 0:
        s.assign(1, 'F');
        break;
    case 1:
        s.assign(1, 'K');
        break;
    case 2:
        s.assign(1, 'R');
        break;
    case 3:
        s.assign(1, 'L');
        break;
    case 4:
        s.assign(1, 'H');
        break;
    case 5:
        s.assign(1, 'G');
        break;
    case 6:
        s.assign(1, 'I');
        break;
    case 7:
        s.assign(1, 'Z');
        break;
    case 16:
        s.assign(1, 'O');
        break;
    case 8:
        s.assign(1, 'P');
        break;
    case 9:
        s.assign(1, 'P');
        break;
    case 10:
        s.assign(1, 'P');
        break;
    case 11:
        s.assign(1, 'P');
        break;
    case 12:
        s.assign(1, 'P');
        break;
    case 13:
        s.assign(1, 'P');
        break;
    case 14:
        s.assign(1, 'P');
        break;
    case 15:
        s.assign(1, 'P');
        break;
    default:
        s.assign(1, ' ');
        break;
    }
    return s + "_Loading";
}

class c_005cf073_Object;
c_005cf073_Object *fn_004bdc4d();
const std::string &fn_004bdcb7();
const std::string &fn_004bdcbd();
const std::string &fn_004bdcc3();
const std::string &fn_004bdcc9();

// HYPOTHESIS: owner class unknown; the two helpers are thiscall methods on the same object.
class c_0047f439_Owner
{
public:
    void fn_0047f4ff();
    void fn_0047f62e(const char *text, const std::string &name);
    void fn_0047f439(int code);
};

// FUNCTION: STARFLEETOP 0x0047f439
void c_0047f439_Owner::fn_0047f439(int code)
{
    std::string s;
    fn_0047f4ff();
    switch (code) {
    case 5: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcbd());
        break;
    }
    case 6: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcc3());
        break;
    }
    case 9: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcc9());
        break;
    }
    default: {
        const char *p = s.c_str();
        fn_004bdc4d();
        fn_0047f62e(p, fn_004bdcb7());
        break;
    }
    }
}
