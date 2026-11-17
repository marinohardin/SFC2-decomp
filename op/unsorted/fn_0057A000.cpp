// HYPOTHESIS: the owning class of these 7-byte member-address getters is not known yet;
// the getters return the address of members at +0x340 and +0x3b4.
class fn_0057a481_object {
public:
    char *fn_0057a481();
    char *fn_0057a488();

    char field_0[0x340];
    char field_340;
    char field_341[0x3b4 - 0x341];
    char field_3b4;
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
