//	fn_004e2895_field148_type.h: the object at offset 0x148 of a phaser bank.
// Evidence:  calls (original name and file unknown)

#ifndef fn_004e2895_field148_type_h
#define fn_004e2895_field148_type_h 1

// HYPOTHESIS: the object at offset 0x148 of a phaser bank, cleared and then filled from a
// per-type table by 0x004e2895 (tPhaserBank) and 0x004e558a (tPhaserXBank). Its class, size
// and the tables' element type are not known.
class fn_004e2895_field148_type
{
public:
    void fn_0050bfaa();
    void fn_004d6fd4();
    void fn_004d6f22(const void* table, int count);
    // HYPOTHESIS: takes a float and returns an int; tBaseWeapon calls it on its member at 0x148.
    int fn_004d7099(float param_1);
};

#endif // fn_004e2895_field148_type_h
