// HYPOTHESIS: a counter bumped by the 3D view manager; type taken from the add.
extern long g_00c7c0b0;

// FUNCTION: STARFLEETOP 0x0069e2a0
void fn_0069e2a0()
{
	g_00c7c0b0++;
}

// HYPOTHESIS: tD3DViewMgr is declared only inside dmp3d/d3d/d3dViewMgr_Tex.cpp; this reduced
// view holds just the members this function touches.
struct tD3DViewMgr_Dev;

struct tD3DViewMgr_DevVtbl
{
	void *slots[22];
	long (__stdcall *SetState)(tD3DViewMgr_Dev *dev, int what, int value);
};

struct tD3DViewMgr_Dev
{
	tD3DViewMgr_DevVtbl *vtbl;
};

class tD3DViewMgr_Sub
{
public:
	void fn_006ab380();

	char field_0[0x1cb0];
	int field_1cb0;
};

class tD3DViewMgr_State
{
public:
	int fn_0069e490(int mode);
	char fn_0069e670();

	char field_0[0x1ed8];
	char field_1ed8;
	char field_1ed9_pad[0x1f4c - 0x1ed9];
	tD3DViewMgr_Sub field_1f4c;
	char field_3bfc_pad[0x3c34 - 0x1f4c - sizeof(tD3DViewMgr_Sub)];
	int field_3c34;
	char field_3c38[0x7f64 - 0x3c38];
	tD3DViewMgr_Dev *field_7f64;
};

// FUNCTION: STARFLEETOP 0x0069e490
int tD3DViewMgr_State::fn_0069e490(int mode)
{
	int value;
	switch (mode)
	{
	case 8:
		value = 1;
		break;
	case 9:
		value = 2;
		break;
	case 10:
		value = 3;
		break;
	default:
		return field_3c34;
	}
	tD3DViewMgr_Sub *sub = &field_1f4c;
	if (sub->field_1cb0 != 0)
		sub->fn_006ab380();
	int old = field_3c34;
	if (field_7f64->vtbl->SetState(field_7f64, 8, value) == 0)
		field_3c34 = mode;
	return old;
}

// FUNCTION: STARFLEETOP 0x0069e670
char tD3DViewMgr_State::fn_0069e670()
{
	return field_1ed8;
}

// FUNCTION: STARFLEETOP 0x0069e950
unsigned long fn_0069e950(unsigned long flags)
{
	unsigned long result = 0;
	if (flags & 0x1)
		result = 0x1;
	if (flags & 0x2)
		result |= 0x2;
	if (flags & 0x4)
		result |= 0x4;
	if (flags & 0x8)
		result |= 0x8;
	if (flags & 0x100)
		result |= 0x10;
	if (flags & 0x200)
		result |= 0x20;
	if (flags & 0x10)
		result |= 0x40;
	if (flags & 0x20)
		result |= 0x80;
	if (flags & 0x40)
		result |= 0x100;
	if (flags & 0x80)
		result |= 0x200;
	if (flags & 0x400)
		result |= 0x400;
	return result;
}
