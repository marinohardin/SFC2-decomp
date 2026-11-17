#include "unsorted/tBaseWeapon.h"
#include "unsorted/tEntity.h"

// HYPOTHESIS: this is tCloakSystem, declared in full only inside unsorted/tCloakSystem.cpp;
// this is the one member used here.
class fn_00438607_class
{
public:
    float mDetermineCloakSeekingWeaponDamageModifier(int param_1, int param_2);
};

// HYPOTHESIS: the entity fn_004249d3 returns; its slot at offset 0x80 gives the cloak system.
class fn_004d7402_entity
{
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11();
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
    virtual void vfunction17();
    virtual void vfunction18();
    virtual void vfunction19();
    virtual void vfunction20();
    virtual void vfunction21();
    virtual void vfunction22();
    virtual void vfunction23();
    virtual void vfunction24();
    virtual void vfunction25();
    virtual void vfunction26();
    virtual void vfunction27();
    virtual void vfunction28();
    virtual void vfunction29();
    virtual void vfunction30();
    virtual void vfunction31();
    virtual void vfunction32();
    virtual fn_00438607_class* vfunction33();
};

// FUNCTION: STARFLEETOP 0x004d7402
int tBaseWeapon::vfunction101(int param_1, int param_2)
{
    fn_004d7402_entity* entity = (fn_004d7402_entity*)gObject_008e1060.fn_004249d3(param_2);
    if (entity != 0)
    {
        fn_00438607_class* cloak = entity->vfunction33();
        if (cloak != 0)
        {
            return (int)(cloak->mDetermineCloakSeekingWeaponDamageModifier(0, 0) * param_1);
        }
    }
    return param_1;
}
