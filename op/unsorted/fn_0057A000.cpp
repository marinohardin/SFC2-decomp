// HYPOTHESIS: the owning class of these 7-byte member-address getters is not known yet;
// the getters return the address of members at +0x340, +0x3b4, +0x4e8, +0x260 and +0x270.
class fn_0057a481_object {
public:
    char *fn_0057a481();
    char *fn_0057a488();
    char *fn_0057a48f();
    char *fn_0057a496();
    char *fn_0057a49d();

    char field_0[0x260];
    char field_260;
    char field_261[0x270 - 0x261];
    char field_270;
    char field_271[0x340 - 0x271];
    char field_340;
    char field_341[0x3b4 - 0x341];
    char field_3b4;
    char field_3b5[0x4e8 - 0x3b5];
    char field_4e8;
};

// FUNCTION: STARFLEETOP 0x0057a481
char *fn_0057a481_object::fn_0057a481()
{
    return &field_340;
}

// FUNCTION: STARFLEETOP 0x0057a488
char *fn_0057a481_object::fn_0057a488()
{
    return &field_3b4;
}

// FUNCTION: STARFLEETOP 0x0057a48f
char *fn_0057a481_object::fn_0057a48f()
{
    return &field_4e8;
}

// FUNCTION: STARFLEETOP 0x0057a496
char *fn_0057a481_object::fn_0057a496()
{
    return &field_260;
}

// FUNCTION: STARFLEETOP 0x0057a49d
char *fn_0057a481_object::fn_0057a49d()
{
    return &field_270;
}
