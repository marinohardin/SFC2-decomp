// HYPOTHESIS: the object held by the global at 0x008cc620; its third virtual loads a texture
// by file name and returns a handle. No header declares it yet; a shared one should replace this.
class fn_008cc620_object_0050678f
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual long vfunction2( const char *name, long a, long b, long c );
};

extern fn_008cc620_object_0050678f *g_008cc620;
extern char g_00967978;
extern long g_00967724;
extern long g_00967728;
extern long g_0096772c;

// FUNCTION: STARFLEETOP 0x0050678f
void fn_0050678f()
{
	if( !g_00967978 )
	{
		g_00967724 = g_008cc620->vfunction2( "assets/textures/expl_00-15.bmp", 1, 0, 0 );
		g_00967728 = g_008cc620->vfunction2( "assets/textures/expl_16-31.bmp", 1, 0, 0 );
		g_0096772c = g_008cc620->vfunction2( "assets/textures/expl_ring16f.bmp", 1, 0, 0 );
	}
	g_00967978 = 1;
}
