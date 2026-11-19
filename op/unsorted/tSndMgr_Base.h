#if !defined( tSndMgr_Base_h )
#define tSndMgr_Base_h

// HYPOTHESIS: only the virtual destructor is declared; the vtable (0x0079faf8) has 28 slots and
// tSndMgr_Miles restates the rest in its own declaration.
class tSndMgr_Base
{
public:
	virtual ~tSndMgr_Base();
};

// The derived destructor (0x0050ede8) inlines this one as a single vtable store.
inline tSndMgr_Base::~tSndMgr_Base()
{
}

#endif
