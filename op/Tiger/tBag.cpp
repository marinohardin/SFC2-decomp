//	tBag.cpp: tItemBag and tBaseItemBag, items stored under C string labels.

#include "Tiger/tBag.h"

#include <stdlib.h>


sMasterBagControlType	tBaseItemBag::smMasterControl;


// MATCH-ONLY: the SDK's Tiger library was compiled with auto-inlining (/Ob2), which inlines
// these helpers into tItemBag; the unit's compile profile has /O2 alone, so they are marked inline.
inline void tBaseItemBag::mInitMasterControl(void)
{
	static int initOnce = 1;

	if (initOnce)
	{
		initOnce = 0;

		smMasterControl.fTotalConstructorsCalled	= 0;
		smMasterControl.fTotalDestructorsCalled		= 0;
		smMasterControl.fTotalBagSize				= 0;
		smMasterControl.fLargestTotalBagSize		= 0;
	}
}

inline void tBaseItemBag::mUpdateMasterConstructorInfo(long delta)
{
	smMasterControl.fTotalConstructorsCalled += (short)delta;
}

inline void tBaseItemBag::mUpdateMasterDestructorInfo(long delta)
{
	smMasterControl.fTotalDestructorsCalled += (short)delta;
}

inline void tBaseItemBag::mUpdateMasterMemInfo(long delta)
{
	smMasterControl.fTotalBagSize += delta;

	if (smMasterControl.fLargestTotalBagSize < smMasterControl.fTotalBagSize)
		smMasterControl.fLargestTotalBagSize = smMasterControl.fTotalBagSize;
}

inline long tBaseItemBag::mTotalMemSize(void)
{
	return smMasterControl.fTotalBagSize;
}

inline byte *tBaseItemBag::mGetMemory(long size)
{
	return (byte *)malloc(size);
}

inline void tBaseItemBag::mFreeMemory(void *ptr, long id)
{
	free(ptr);
}


// FUNCTION: STARFLEETOP 0x0066d680
tItemBag::tItemBag()
{
	tBaseItemBag::mInitMasterControl();
	tBaseItemBag::mUpdateMasterConstructorInfo(1);

	fNumItems		= 0;
	fBagItemList	= NULL;

	tBaseItemBag::mUpdateMasterMemInfo(sizeof(tItemBag));
}

// FUNCTION: STARFLEETOP 0x0066d6f0
// HYPOTHESIS: compiler-generated scalar deleting destructor of the virtual destructor,
// annotated at its own address.
void *tItemBag::scalar_deleting_destructor(unsigned char flags)
{
	// MATCH-ONLY: spells out the compiler-generated deleting destructor, since the
	// harness annotates a function by definition and cannot name the generated one.
	this->tItemBag::~tItemBag();
	if (flags & 1)
		// HYPOTHESIS: the exe calls the global operator delete (0x0072eec0) here, not the
		// class's own operator delete at 0x006a2f10.
		::operator delete(this);
	return this;
}

// FUNCTION: STARFLEETOP 0x0066d710
tItemBag::~tItemBag()
{
	if (fNumItems >= 0)
	{
		while (fBagItemList)
		{
			tBaseItemBag	*next = fBagItemList->fNext;

			if (fBagItemList->fLabel)
			{
				tBaseItemBag::mUpdateMasterMemInfo(-(long)(strlen(fBagItemList->fLabel) + 1));
				tBaseItemBag::mFreeMemory(fBagItemList->fLabel);
			}

			delete fBagItemList;
			fBagItemList = next;
		}

		tBaseItemBag::mUpdateMasterDestructorInfo(1);
		tBaseItemBag::mUpdateMasterMemInfo(-(long)sizeof(tItemBag));
	}
	else
	{
		assert(tBaseItemBag::mTotalMemSize() == 0);
	}
}

// FUNCTION: STARFLEETOP 0x0066d7e0
long tItemBag::mAddToList(tBaseItemBag *Item, char *label)
{
	// the original keeps the length in a byte, so a label of 256 or more characters is truncated

	byte	size = strlen(label) + 1;

	Item->fLabel = (char *)tBaseItemBag::mGetMemory(size);
	if (Item->fLabel)
	{
		tBaseItemBag::mUpdateMasterMemInfo(size);

		strcpy(Item->fLabel, label);

		tBaseItemBag	*oldList = fBagItemList;
		fBagItemList	= Item;
		Item->fNext		= oldList;

		return 1;
	}

	return 0;
}

// FUNCTION: STARFLEETOP 0x0066d880
tBaseItemBag *tItemBag::mFindItemWithLabel(char *label)
{
	tBaseItemBag	*found	= NULL;
	tBaseItemBag	*item	= fBagItemList;

	while (found == NULL && item != NULL)
	{
		if (strcmp(item->fLabel, label) == 0)
			found = item;

		item = item->fNext;
	}

	return found;
}
