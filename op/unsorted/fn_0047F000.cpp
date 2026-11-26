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
