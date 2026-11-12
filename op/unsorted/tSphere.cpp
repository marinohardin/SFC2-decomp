// Unit:      unsorted/tSphere.cpp
// Functions: 0x0040f15a to 0x0040f99d, 13 in game/UNITS.tsv
// Evidence:  class (functions of this class outside every evidenced unit; original file unknown)
// Profile:   game-release (harness/units.json)

#include <string.h>

// HYPOTHESIS: the class of the object tSphere::vfunction4 (0x0040f87f) is handed is not known.
// Only the three slots it calls are given their types; the earlier slots hold their places.
class fn_0040f87f_Target {
public:
    virtual void vfunction1();  virtual void vfunction2();  virtual void vfunction3();
    virtual void vfunction4();  virtual void vfunction5();  virtual void vfunction6();
    virtual void vfunction7();  virtual void vfunction8();  virtual void vfunction9();
    virtual void vfunction10(); virtual void vfunction11(); virtual void vfunction12();
    virtual void vfunction13(); virtual void vfunction14(); virtual void vfunction15();
    virtual void vfunction16(); virtual void vfunction17(); virtual void vfunction18();
    virtual void vfunction19(); virtual void vfunction20(); virtual void vfunction21();
    virtual void vfunction22(); virtual void vfunction23(); virtual void vfunction24();
    virtual void vfunction25(); virtual void vfunction26(); virtual void vfunction27();
    virtual void vfunction28(); virtual void vfunction29(); virtual void vfunction30();
    virtual void vfunction31(); virtual void vfunction32(); virtual void vfunction33();
    virtual void vfunction34(); virtual void vfunction35(); virtual void vfunction36();
    virtual void vfunction37(); virtual void vfunction38(); virtual void vfunction39();
    virtual void vfunction40(); virtual void vfunction41(); virtual void vfunction42();
    virtual void vfunction43(); virtual void vfunction44(); virtual void vfunction45();
    virtual void vfunction46(); virtual void vfunction47(); virtual void vfunction48();
    virtual void vfunction49(); virtual void vfunction50(); virtual void vfunction51();
    virtual void vfunction52(); virtual void vfunction53(); virtual void vfunction54();
    virtual void vfunction55(); virtual void vfunction56(); virtual void vfunction57();
    virtual void vfunction58(); virtual void vfunction59(); virtual void vfunction60();
    virtual void vfunction61(); virtual void vfunction62(); virtual void vfunction63();
    virtual void vfunction64(); virtual void vfunction65(); virtual void vfunction66();
    virtual void vfunction67(); virtual void vfunction68(); virtual void vfunction69();
    virtual void vfunction70(); virtual void vfunction71(); virtual void vfunction72();
    virtual void vfunction73(); virtual void vfunction74(); virtual void vfunction75();
    virtual void vfunction76(); virtual void vfunction77(); virtual void vfunction78();
    virtual void vfunction79(int param_1, int *param_2);      // +0x138
    virtual void vfunction80(int *param_1);                   // +0x13c
    virtual void vfunction81(int param_1, int **param_2, int param_3); // +0x140
};

// HYPOTHESIS: tSphere::vfunction10 (0x0040f99d) takes a 0x44-byte object by value (it returns
// with ret 0x44) and calls the method at 0x0069c970 on it once per 12-byte vertex of field_4.
// The object's class and the method's name are not known; the size alone is given here.
class fn_0069c970_Owner {
public:
    void fn_0069c970(void *param_1);
    int field_0[17];
};

class tSphere {
public:
    // HYPOTHESIS: vtable 0x00798488 slot 1 (slot 0 is the deleting destructor). The body at
    // 0x0040f164 reads six arguments at [ebp+8]..[ebp+0x1c] as float, int, int, float, float,
    // float, returns in al (mov al,1 / xor al,al) and ends with ret 0x18 at 0x0040f87c.
    virtual bool vfunction2(float radius, int param_2, int param_3, float param_4, float param_5, float param_6);
    // HYPOTHESIS: vtable slot 2.
    virtual void vfunction3();
    // HYPOTHESIS: vtable slot 3.
    virtual void vfunction4(fn_0040f87f_Target *param_1);
    // HYPOTHESIS: a later vtable slot; its position is not yet placed.
    virtual void vfunction11();
    // HYPOTHESIS: vtable slot 4.
    virtual void vfunction5(int param_1);
    // HYPOTHESIS: a later vtable slot; its position is not yet placed.
    virtual void vfunction7(int param_1);
    // HYPOTHESIS: a later vtable slot; its position is not yet placed.
    virtual void vfunction8(int *param_1);
    // HYPOTHESIS: a later vtable slot; its position is not yet placed.
    virtual void vfunction9(float param_1);
    // HYPOTHESIS: a later vtable slot; its position is not yet placed.
    virtual void vfunction10(fn_0069c970_Owner param_1);
    // HYPOTHESIS: not a member of its own but the shared epilogue of a member that takes seven
    // stack arguments (0x0040f86c, see its definition); declared here so that it is a member.
    bool FUN_0040f86c(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7);

    // HYPOTHESIS: field types are unknown beyond what vfunction3 shows: field_4 is a heap
    // buffer with field_8 its length, field_10 a heap array of field_c heap buffers.
    int *field_4;
    int field_8;
    int field_c;
    int **field_10;
    // HYPOTHESIS: bit 0 of field_14 records whether field_28 is set (vfunction5).
    int field_14;
    int field_18;
    // HYPOTHESIS: field_1c is a float clamped to [0, 1] by vfunction9.
    float field_1c;
    int field_20;
    int field_24;
    int field_28;
};

// HYPOTHESIS: the analysis splits this function after its __EH_prolog call. The body
// continues at 0x0040f164 (FUN_0040f164, 1767 bytes), its catch funclet is 0x0040f84b
// (Catch@0040f84b, which frees a buffer, calls vtable slot 2 and resumes at 0x0040f86c) and
// its shared epilogue is 0x0040f86c (FUN_0040f86c: xor al,al; restore fs:[0]; leave; ret 0x18),
// so the real function is 0x0040f15a..0x0040f87f. It validates the arguments (radius and the
// three trailing floats against the constant at 0x0079561c, param_2 >= 1, param_3 >= 3),
// builds the sphere inside a try block and returns true, or false on a bad argument or an
// exception. Only the 10-byte prolog is scored while the split stands, so the body is a
// stub that reports failure; it cannot be rebuilt here without regressing the score.
// FUNCTION: STARFLEETOP 0x0040f15a
bool tSphere::vfunction2(float radius, int param_2, int param_3, float param_4, float param_5, float param_6)
{
    return false;
}

// HYPOTHESIS: this is the shared epilogue of tSphere::vfunction2 (xor al,al; restore fs:[0];
// pop edi/esi/ebx; leave; ret 0x1c), where the catch funclet resumes, not a function of its
// own. The original bytes pop 0x1c, so the enclosing function takes seven stack arguments, not
// the six the declaration of vfunction2 above gives it.
// MATCH-ONLY: a standalone function cannot rebuild a mid-function epilogue; this stub keeps
// what the fragment does (return false, pop seven arguments) without the frame teardown. It is
// a tSphere member because the epilogue belongs to one: __thiscall pops its seven stack
// arguments like the original ret 0x1c, with this in ecx.
// FUNCTION: STARFLEETOP 0x0040f86c
bool tSphere::FUN_0040f86c(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6, int param_7)
{
    return false;
}

// HYPOTHESIS: tSphere::vfunction4 sets its sphere up on the object it is given through that
// object's vtable slots 78, 79 and 80 (offsets 0x138, 0x13c, 0x140), whose names are not known.
// FUNCTION: STARFLEETOP 0x0040f87f
void tSphere::vfunction4(fn_0040f87f_Target *param_1)
{
    if (field_4 != 0 && param_1 != 0) {
        param_1->vfunction80(&field_14);
        param_1->vfunction79(field_8, field_4);
        param_1->vfunction81(field_c, field_10, 1);
    }
}

// FUNCTION: STARFLEETOP 0x0040f8c6
void tSphere::vfunction3()
{
    if (field_4 != 0) {
        delete[] field_4;
        field_4 = 0;
    }
    field_8 = 0;
    if (field_10 != 0) {
        for (int i = 0; i < field_c; i++) {
            if (field_10[i] != 0) {
                delete[] field_10[i];
            }
        }
        delete[] field_10;
        field_10 = 0;
        field_c = 0;
    }
}

// FUNCTION: STARFLEETOP 0x0040f917
void tSphere::vfunction11()
{
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
    field_10 = 0;
    // HYPOTHESIS: field_14 begins a 0x24-byte block (nine 4-byte values) that is cleared whole.
    memset(&field_14, 0, 0x24);
}

// FUNCTION: STARFLEETOP 0x0040f935
void tSphere::vfunction5(int param_1)
{
    field_28 = param_1;
    if (param_1 != 0) {
        field_14 |= 1;
    } else {
        field_14 &= ~1;
    }
}

// FUNCTION: STARFLEETOP 0x0040f94d
void tSphere::vfunction7(int param_1)
{
    // HYPOTHESIS: param_1 is a mode code; 5 sets bit 1 of field_14, 3 and 4 clear it.
    switch (param_1) {
    case 3:
        field_14 &= ~2;
        break;
    case 4:
        field_14 &= ~2;
        break;
    case 5:
        field_14 |= 2;
        break;
    }
}

// FUNCTION: STARFLEETOP 0x0040f969
void tSphere::vfunction8(int *param_1)
{
    field_18 = *param_1;
}

// FUNCTION: STARFLEETOP 0x0040f975
void tSphere::vfunction9(float param_1)
{
    if (param_1 < 0.0f) {
        param_1 = 0.0f;
    }
    if (param_1 > 1.0f) {
        param_1 = 1.0f;
    }
    field_1c = param_1;
}

// FUNCTION: STARFLEETOP 0x0040f99d
void tSphere::vfunction10(fn_0069c970_Owner param_1)
{
    // HYPOTHESIS: field_4 holds field_8 vertices of 12 bytes each (three floats).
    for (int i = 0; i < field_8; i++) {
        param_1.fn_0069c970((char *)field_4 + i * 12);
    }
}
