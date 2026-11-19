#ifndef FN_0090BED8_TYPE_H
#define FN_0090BED8_TYPE_H

// HYPOTHESIS: a global object at 0x0090bed8 whose getter at 0x00518603 returns a float
// scale that 0x004e23b5 and 0x004e57c0 apply to their requests.
class fn_0090bed8_type
{
public:
    float fn_00518603();
    // HYPOTHESIS: returns a time in seconds; 0x0060c1c7 truncates it and prints it as h:m:s.
    float fn_0045d50a();
};
extern fn_0090bed8_type g_0090bed8;

#endif
