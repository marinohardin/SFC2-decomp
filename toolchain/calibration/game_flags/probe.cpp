// Probes for the game's compile flags (toolchain/calibration/game_flags.md): three small
// functions of the game's own code, written to the behaviour of the original at the address
// named on each; every probe's name carries that address. Field and global names are
// placeholders, not recovered identifiers.

extern char g_byte_00898c40;
extern int g_int_00898c44;

// STARFLEETOP 0x0040c832
void probe_0040c832()
{
    g_byte_00898c40 = 0;
    g_int_00898c44 = 0;
}

struct Probe_0040f975 {
    char pad[0x1c];
    float fValue;
    void set(float v);
};

// STARFLEETOP 0x0040f975
void Probe_0040f975::set(float v)
{
    if (v < 0.0f)
        v = 0.0f;
    if (v > 1.0f)
        v = 1.0f;
    fValue = v;
}

struct Probe_00419c87 {
    char pad[0x870];
    int fKind;
    int fState;
    bool test();
};

// STARFLEETOP 0x00419c87
bool Probe_00419c87::test()
{
    if ((fKind == 22 || fKind == 23) && fState != 1)
        return true;
    return false;
}
