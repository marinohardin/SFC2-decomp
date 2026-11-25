// Unit:      unsorted/fn_00427000.cpp
// Functions: 0x00427145 to 0x00427160, 2 in op/UNITS.tsv
// Profile:   game-release (harness/units.json)

// HYPOTHESIS: the owner class, the member at +0x18 and the callee's class
// (tMatchedMission, declared only in another unit) are unknown.
class fn_00427145_Owner {
public:
    char pad[0x18];
    char m18[4];
    fn_00427145_Owner *fn_00427145(fn_00427145_Owner *p);
    void fn_00427160(void *p);
};

// FUNCTION: STARFLEETOP 0x00427145
fn_00427145_Owner *fn_00427145_Owner::fn_00427145(fn_00427145_Owner *p)
{
    // MATCH-ONLY: unused zeroed local the original's frame shows
    volatile int zero = 0;
    p->fn_00427160(m18);
    return p;
}
