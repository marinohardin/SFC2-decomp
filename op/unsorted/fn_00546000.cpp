// Unit:      unsorted/fn_00546000.cpp
// Functions with no unit or class evidence on the 4 KB page 0x00546000; original file unknown.

// HYPOTHESIS: the class that owns 0x00451045; name and parameter types unknown.
class c_00451045
{
public:
    double fn_00451045(int param_1, int param_2);
};

// HYPOTHESIS: the class that owns 0x00546027; only the member at +0x90 is known.
class c_00546027
{
public:
    double fn_00546027(int param_1, int param_2);
    int fn_0054603d(double param_1);
    // HYPOTHESIS: 0x00410223 is called on the same object; it may belong to a base class.
    int fn_00410223();

    char field_0[0x90];
    c_00451045 field_90;
};

// FUNCTION: STARFLEETOP 0x00546027
double c_00546027::fn_00546027(int param_1, int param_2)
{
    return field_90.fn_00451045(param_1, param_2);
}

// FUNCTION: STARFLEETOP 0x0054603d
int c_00546027::fn_0054603d(double param_1)
{
    return fn_00546027(0, 0) * fn_00410223() * param_1;
}
