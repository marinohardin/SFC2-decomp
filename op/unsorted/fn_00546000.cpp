// Unit:      unsorted/fn_00546000.cpp
// Functions with no unit or class evidence on the 4 KB page 0x00546000; original file unknown.

// HYPOTHESIS: the class that owns 0x00451045; name and parameter types unknown.
class c_00451045
{
public:
    void fn_00451045(int param_1, int param_2);
};

// HYPOTHESIS: the class that owns 0x00546027; only the member at +0x90 is known.
class c_00546027
{
public:
    void fn_00546027(int param_1, int param_2);

    char field_0[0x90];
    c_00451045 field_90;
};

// FUNCTION: STARFLEETOP 0x00546027
void c_00546027::fn_00546027(int param_1, int param_2)
{
    field_90.fn_00451045(param_1, param_2);
}
