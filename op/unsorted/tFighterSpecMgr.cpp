// HYPOTHESIS: layout from the constructor at 0x00401c9c (vtable 0x00795654); +0x04 holds a
// 12-byte member with a destructor, +0x10 and +0x28 look like inlined container headers.
struct fn_00401c9c_base
{
    ~fn_00401c9c_base();

    long m00;
    long m04;
    long m08;
};

inline fn_00401c9c_base::~fn_00401c9c_base()
{
}

class tFighterSpecMgr
{
public:
    tFighterSpecMgr();
    virtual ~tFighterSpecMgr();
    void fn_00401ff6();

    fn_00401c9c_base m04;
    char m10;
    long m14;
    long m18;
    long m1c;
    long m20;
    long m24;
    char m28;
    long m2c;
    long m30;
    long m34;
};

// FUNCTION: STARFLEETOP 0x00401c9c
tFighterSpecMgr::tFighterSpecMgr()
{
    char alloc;
    m10 = alloc;
    m14 = 0;
    m18 = 0;
    m1c = 0;
    m20 = 0;
    m24 = 0;
    m28 = alloc;
    m2c = 0;
    m30 = 0;
    m34 = 0;
    fn_00401ff6();
}
