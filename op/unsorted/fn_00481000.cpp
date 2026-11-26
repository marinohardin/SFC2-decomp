// HYPOTHESIS: a container with a 12-byte header and 0x20-byte elements starting at +0xc, and an
// element count at +0x100c; its original class is not known.
class fn_0048179b_Object
{
public:
	char* fn_0048179b();
	char	fUnknown_00[0xc];
	char	fElements[0x1000];
	int		fCount;
};

// FUNCTION: STARFLEETOP 0x0048179b
char* fn_0048179b_Object::fn_0048179b()
{
	return (char*)this + 0xc + fCount * 0x20;
}
