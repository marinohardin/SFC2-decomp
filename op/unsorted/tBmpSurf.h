#ifndef TBMPSURF_H
#define TBMPSURF_H

// The class name is the exe's RTTI (.?AVtBmpSurf@@).
// HYPOTHESIS: a surface over a locked tTexBmp's pixels; built on the stack by
// tSFCTexMgrEx::vfunction4, which reserves 0x6c bytes for it, the vtable pointer first. The
// constructor and the virtual destructor are named by their SDK symbols; the fields are unknown.
class tTexBmp;

class tBmpSurf
{
public:
	tBmpSurf( tTexBmp &bmp );
	virtual ~tBmpSurf();

	char	pad04[0x68];
};

#endif
