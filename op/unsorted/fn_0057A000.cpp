// HYPOTHESIS: the owning class of these 7-byte member-address getters is not known yet;
// the getter returns the address of a member at +0x340.
class fn_0057a481_object {
public:
    char *fn_0057a481();

    char field_0[0x340];
    char field_340;
};

// FUNCTION: STARFLEETOP 0x0057a481
char *fn_0057a481_object::fn_0057a481()
{
    return &field_340;
}
