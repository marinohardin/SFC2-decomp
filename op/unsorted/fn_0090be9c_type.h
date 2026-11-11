//	fn_0090be9c_type.h: the global object at 0x0090be9c.
// Evidence:  calls (original name and file unknown)

#ifndef fn_0090be9c_type_h
#define fn_0090be9c_type_h 1

// HYPOTHESIS: a global object at 0x0090be9c; vftable slot 6 returns an unsigned count and
// +0x20 holds an unsigned total. tCloakSystem's 0x00438607 and the phaser banks divide one by
// the other.
class fn_0090be9c_type
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual unsigned int vfunction7(); // slot 6

    char pad04[0x20 - 0x04];
    unsigned int field_20;
};
extern fn_0090be9c_type* g_0090be9c;

#endif // fn_0090be9c_type_h
