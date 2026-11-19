// HYPOTHESIS: the class layout and the destructor body (the original destructor at 0x00675250
// is not recovered yet) are placeholders; only the deleting destructor is claimed here.
class tSurface {
public:
    virtual ~tSurface();
};

// FUNCTION: STARFLEETOP 0x00675060
tSurface::~tSurface()
{
}
