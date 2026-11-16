#ifndef FN_004D288F_OBJECT_H
#define FN_004D288F_OBJECT_H

#include <string>

// HYPOTHESIS: the object fn_004d288f returns (0x004d288f looks up an object by a pointer key);
// two of its functions are known, one taking a std::string by value (0x004d2573) and one taking
// a pointer (0x004d2521).
class fn_004d288f_object {
public:
    void fn_004d2573(std::string text);
    void fn_004d2521(const void *arg);
};

#endif
