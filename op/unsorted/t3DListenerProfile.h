//	t3DListenerProfile.h: t3DListenerProfile and the 12-byte value it holds.
// Evidence:  class (original file unknown)

#ifndef t3DListenerProfile_h
#define t3DListenerProfile_h 1

// HYPOTHESIS: a 12-byte value copied whole, likely a three-component vector; name unknown.
struct t3DListenerProfile_field_08 {
    int field_00;
    int field_04;
    int field_08;
};

class t3DListenerProfile {
public:
    virtual ~t3DListenerProfile();
    virtual void fn_004355ce(const t3DListenerProfile_field_08 *param_1);
    virtual void fn_004355e6(int param_1, int param_2, int param_3);
    virtual t3DListenerProfile_field_08 *fn_004355fe();
    virtual void fn_00435602(const t3DListenerProfile_field_08 *param_1);
    virtual void fn_0043561a(int param_1, int param_2, int param_3);
    virtual t3DListenerProfile_field_08 *fn_00435632();
    virtual void fn_00435636(const t3DListenerProfile_field_08 *param_1);
    virtual void fn_0043564e(int param_1, int param_2, int param_3);
    virtual t3DListenerProfile_field_08 *fn_00435666();
    virtual void fn_0043566a(const t3DListenerProfile_field_08 *param_1);
    virtual void fn_00435682(int param_1, int param_2, int param_3);
    virtual t3DListenerProfile_field_08 *fn_0043569a();
    virtual void fn_0043569e(double param_1);
    virtual double fn_004356a8();

    int field_04;
    t3DListenerProfile_field_08 field_08;
    // HYPOTHESIS: a second 12-byte value of the same kind as field_08, set by 0x00435602.
    t3DListenerProfile_field_08 field_14;
    // HYPOTHESIS: a third 12-byte value of the same kind, set by 0x00435636.
    t3DListenerProfile_field_08 field_20;
    // HYPOTHESIS: a fourth 12-byte value of the same kind, set by 0x0043566a.
    t3DListenerProfile_field_08 field_2c;
    // HYPOTHESIS: a double at +0x38, set by 0x0043569e. Declared as two ints because a double
    // member would make the class 8-byte aligned and pad the vtable pointer, moving every field
    // by 4; the original layout has no such padding (packing unknown).
    int field_38[2];
};

#endif
