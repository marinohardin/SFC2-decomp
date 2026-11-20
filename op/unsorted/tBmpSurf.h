#ifndef TBMPSURF_H
#define TBMPSURF_H

// The class name is the exe's RTTI (.?AVtBmpSurf@@).
// HYPOTHESIS: a surface over a locked tTexBmp's pixels; built on the stack by
// tSFCTexMgrEx::vfunction4, which reserves 0x6c bytes for it. The fields are unknown.
class tTexBmp;

class tBmpSurf
{
public:
	tBmpSurf( tTexBmp *bmp );
	~tBmpSurf();

	char	pad00[0x6c];
};

#endif
