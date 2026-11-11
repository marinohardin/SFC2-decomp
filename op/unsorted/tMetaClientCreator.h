#ifndef tMetaClientCreator_h
#define tMetaClientCreator_h

// HYPOTHESIS: the layout is known only by size (0x44 bytes, from the stack frame of the function
// at 0x006082ef, which holds six derived objects side by side) and by the vtable at 0x007b6184:
// slot 0 the destructor, slot 1 a function every derived class inherits (0x00607460), slot 2
// pure. The return type of slot 1 is a guess: one 32-bit value that the caller stores.
class tMetaClientCreator {
public:
    tMetaClientCreator();
    virtual ~tMetaClientCreator();
    virtual void *vfunction2();
    virtual void vfunction3() = 0;

    char m_0x04[0x40];
};

#endif
