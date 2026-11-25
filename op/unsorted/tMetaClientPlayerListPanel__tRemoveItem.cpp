#include "Common/Thread/Optex.h"

// HYPOTHESIS: the original is tMetaClientPlayerListPanel::tRemoveItem; the outer class is declared
// only in another unit and has no header yet, so the item class stands alone here. Only what this
// destructor touches is declared: a cleanup member at 0x0060432e and a tOptex at +0x4.
// HYPOTHESIS: a base holding the word at +0, then tOptex as a second base at +0x4.
class tRemoveItemBase
{
protected:
	int32u	field_00;
};

class tRemoveItem : public tRemoveItemBase, public tOptex
{
public:
	~tRemoveItem();
	void fn_0060432e();
};

// The SDK's inline tOptex destructor, as in fn_00543000.cpp: with its body in view the compiler
// sets no unwind state before calling it.
inline tOptex::~tOptex()
{
	if (fUse != tThreadState::kIgnoreThreadObject) {
		if (fUseCriticalSections && fpSharedInfo != NULL)
			::DeleteCriticalSection(&fpSharedInfo->fCriticalSection);
		if (fEvent != NULL)
			::CloseHandle(fEvent);
		if (fFileMapping == NULL) {
			delete fpSharedInfo;
		} else {
			::UnmapViewOfFile(fpSharedInfo);
			::CloseHandle(fFileMapping);
		}
	}
}

// FUNCTION: STARFLEETOP 0x006041a1
tRemoveItem::~tRemoveItem()
{
	fn_0060432e();
}
