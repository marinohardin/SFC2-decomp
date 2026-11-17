// Unit:      unsorted/fn_00436000.cpp
// Functions: 0x00436006 to 0x004364b6, 37 in op/UNITS.tsv
// Evidence:  page (functions with no unit or class evidence, by 4 KB page; original file unknown)
// Profile:   game-release (harness/units.json)

#include "Common/BaseTypes.h"

class tPath;
class twPath;
class tSession;
class tFSSession;
class tVolIterator;
class tVolIterParent;
class tDirIterator;
class tDirIterParent;
class tPathIterator;
class tPathIterParent;

namespace std
{
	template <class _Ty> struct less;
}

// HYPOTHESIS: the full tCountMap is declared only inside op/Tiger/tgfsys.cpp; this unit needs
// just the virtual destructor, whose deleting form is the function below.
template <class tKeyType, class tValueType, class tCompare>
class tCountMap
{
public:
	tCountMap();
	virtual ~tCountMap();

	void		*fHead;		// +0x04
	void		*fTail;		// +0x08
};

// HYPOTHESIS: the full tValueCountMap is declared only inside op/Tiger/tgfsys.cpp, where its
// leading word is not recovered; the vtable store below shows it is a virtual destructor's.
template <class tKeyType, class tValueType, class tCompare>
class tValueCountMap
{
public:
	tValueCountMap() {}
	virtual ~tValueCountMap();

	tCountMap<tKeyType,tValueType,tCompare>	fMaps[256];		// +0x04
};

// HYPOTHESIS: the full tCookieManager is declared only inside op/Tiger/tgfsys.cpp. The function
// below stores the map's vtable at +0x04 after building its 256 maps and then clears +0x00, which
// is the cookie manager's constructor with tValueCountMap's inlined, not tValueCountMap's own.
template <class tCookieType, class tObjectType>
class tCookieManager
{
public:
	tCookieManager();

	int32u											f_00;
	tValueCountMap<tCookieType,tObjectType,std::less<tCookieType> >	fValues;	// +0x04
};

// HYPOTHESIS: the destructor body (0x00435e84) is not recovered; empty here.
// FUNCTION: STARFLEETOP 0x004361cb
template <>
tCountMap<tPath,twPath,std::less<tPath> >::~tCountMap()
{
}

// FUNCTION: STARFLEETOP 0x004361e7
template <>
tCookieManager<tSession,tFSSession>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x00436212
template <>
tCookieManager<tVolIterator,tVolIterParent>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x0043623d
template <>
tCookieManager<tDirIterator,tDirIterParent>::tCookieManager()
{
	f_00 = 0;
}

// FUNCTION: STARFLEETOP 0x00436268
template <>
tCookieManager<tPathIterator,tPathIterParent>::tCookieManager()
{
	f_00 = 0;
}

// HYPOTHESIS: a singly linked list whose nodes have a virtual destructor and their link at
// +0x08; neither class is named by the exe.
class tNode_00436293
{
public:
	virtual ~tNode_00436293();

	int32u			f_04;
	tNode_00436293	*fNext;		// +0x08
};

class tList_00436293
{
public:
	void	FUN_00436293();

	int32u			f_00;
	tNode_00436293	*fHead;		// +0x04
	tNode_00436293	*fTail;		// +0x08
};

// FUNCTION: STARFLEETOP 0x00436293
void tList_00436293::FUN_00436293()
{
	tNode_00436293	*node = fHead;

	fHead = node->fNext;
	if ( fHead == 0 )
		fTail = 0;
	delete node;
}
