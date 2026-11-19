#ifndef T3DVISUAL_H
#define T3DVISUAL_H

// RTTI names t3DVisual (vtable 0x00798528, 27 slots). Only the destructor is declared
// so far; the other slots are not written yet.
// HYPOTHESIS: m04 belongs to t3DVisual; its inline constructor sets it to 1.
class t3DVisual
{
public:
	t3DVisual();
	virtual ~t3DVisual();

	int m04;
};

inline t3DVisual::t3DVisual()
{
	m04 = 1;
}

// 0x0042f34a is the out-of-line copy; derived destructors inline it.
inline t3DVisual::~t3DVisual()
{
}

#endif
