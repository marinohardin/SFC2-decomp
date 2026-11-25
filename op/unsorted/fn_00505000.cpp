// FUNCTION: STARFLEETOP 0x005050e2
float fn_005050e2(float param_1)
{
    return param_1 * 0.00390625f;
}

// HYPOTHESIS: the object held by the global at 0x008cc620; its third virtual loads a texture
// by file name and returns a handle. No header declares it yet; a shared one should replace this.
class fn_008cc620_object_005050ed
{
public:
    virtual void vfunction0();
    virtual void vfunction1();
    virtual long vfunction2(const char *name, long a, long b, long c);
};

extern fn_008cc620_object_005050ed *g_008cc620;
extern char g_00967698;
extern long g_0085829c;
extern long g_008582a0;

// FUNCTION: STARFLEETOP 0x005050ed
void fn_005050ed()
{
    if (!g_00967698)
    {
        g_0085829c = g_008cc620->vfunction2("assets/textures/pulsar_1.bmp", 1, 0, 0);
        g_008582a0 = g_008cc620->vfunction2("assets/textures/pulsar_2.bmp", 1, 0, 0);
    }
    g_00967698 = 1;
}
