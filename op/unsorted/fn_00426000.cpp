// HYPOTHESIS: the insertion sort of the standard library's sort, instantiated for unsigned int.
// FUNCTION: STARFLEETOP 0x004260a4
void fn_004260a4(unsigned int* first, unsigned int* last)
{
	if (first == last)
		return;
	for (unsigned int* i = first + 1; i != last; ++i)
	{
		unsigned int val = *i;
		if (!(val < *first))
		{
			unsigned int* p = i;
			unsigned int* q = p - 1;
			while (val < *q)
			{
				*p = *q;
				p = q;
				--q;
			}
			*p = val;
		}
		else
		{
			unsigned int* src = i;
			unsigned int* dst = i + 1;
			while (first != src)
				*--dst = *--src;
			*first = val;
		}
	}
}

// HYPOTHESIS: the object held by the global at 0x008cc620 (also declared inside fn_00496000.cpp);
// its third virtual loads a texture by file name and returns a handle.
class fn_00426116_Loader
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual long vfunction2(const char* name, long a, long b, long c);
};

extern fn_00426116_Loader* g_008cc620;
extern char g_008f5230;
// HYPOTHESIS: the background texture handle.
extern long g_008f522c;

// FUNCTION: STARFLEETOP 0x00426116
void fn_00426116()
{
	if (!g_008f5230)
	{
		g_008f522c = g_008cc620->vfunction2("assets/textures/skybgnd-mono.BMP", 1, 0, 0);
	}
	g_008f5230 = 1;
}
