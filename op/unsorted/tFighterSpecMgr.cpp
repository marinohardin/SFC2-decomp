#include <vector>

class tFighterSpecMgr
{
public:
    tFighterSpecMgr();
    virtual ~tFighterSpecMgr();
    void fn_00401ff6();

    long m04;
    long m08;
    long m0c;
    // HYPOTHESIS: +0x10 and +0x28 are std::vectors; element types unknown.
    std::vector<long> m10;
    long m20;
    long m24;
    std::vector<long> m28;
};

// FUNCTION: STARFLEETOP 0x00401c9c
tFighterSpecMgr::tFighterSpecMgr()
    : m20(0), m24(0)
{
    fn_00401ff6();
}
