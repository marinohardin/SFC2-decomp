// HYPOTHESIS: layout from the one member read here; the class has a vtable at +0 and the
// reference count at +4. The member name is ours.
class tGLFontEx
{
public:
    virtual long mAddRef();

    long mRefCount;
};

// FUNCTION: STARFLEETOP 0x006b32c0
long tGLFontEx::mAddRef()
{
    return ++mRefCount;
}
