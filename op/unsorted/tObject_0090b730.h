#ifndef TOBJECT_0090B730_H
#define TOBJECT_0090B730_H

#include <vector>

#include "Tiger/tstring.h"
#include "unsorted/fn_005717f7_vector.h"

class C_0060cf03;
class C_004729a6;

// HYPOTHESIS: the class at +0x24 of tObject_0090b730, built by 0x00472518 (0x704 bytes); name and
// members unknown.
class C_00472518
{
public:
    C_00472518();
    ~C_00472518();
    C_00472518 &operator=(const C_00472518 &other);
    // HYPOTHESIS: the layout follows the copy at 0x0060c892; member meanings are unknown.
    int m_00;
    int m_04;
    int m_08;
    int m_0c;
    tString m_10[6];
    int m_100;
    int m_104[6];
    int m_11c[6];
    int m_134[6];
    int m_14c[6];
    tString m_164[18];
    tString m_434[18];
};

// HYPOTHESIS: the class 0x0060c7dc constructs; 0x0060ca83 stores a table entry at +0x18. It is
// the class of the global at 0x0090b730: other units call 0x0060ca42..0x0060cac6 and 0x005c7fb3
// on that global, and 0x0060cac6 reaches the vector through it.
class tObject_0090b730
{
public:
    tObject_0090b730();
    ~tObject_0090b730();
    long fn_005c7fb3();
    void fn_0060ca20();
    void fn_0060ca42(int value);
    int fn_0060ca4f();
    bool fn_0060ca56();
    bool fn_0060ca6c();
    char *fn_0060ca77();
    bool fn_0060ca7d();
    void fn_0060ca83(int index);
    void fn_0060ca94(int value);
    int fn_0060ca9e();
    void fn_0060cac6();
    void fn_0060cb1b();
    void fn_0060c883(const C_00472518 &value);
    C_00472518 &fn_0060c97e();
    char *fn_0060c982(const char *name);
    C_0060cf03 *m_00;
    int m_04;
    // HYPOTHESIS: a vector of C_004729a6 pointers; 0x0060c841 destroys it through 0x00571a7a,
    // so it is declared as fn_005717f7_vector.
    fn_005717f7_vector m_08;
    int m_18;
    int m_1c;
    bool m_20;
    C_00472518 m_24;
    int m_728;
};

extern tObject_0090b730 gObject_0090b730;

// MATCH-ONLY: the library vector of C_004729a6 pointers that m_08 stands for; its empty, back,
// erase and clear are the ones 0x0060cac6 and 0x0060cb1b call.
inline std::vector<C_004729a6 *> &fn_0060cac6_teams( fn_005717f7_vector &v )
{
    return *(std::vector<C_004729a6 *> *)&v;
}

#endif
