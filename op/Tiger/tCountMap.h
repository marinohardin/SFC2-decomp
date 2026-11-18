//	tCountMap.h: tCountMap, tValueCountMap and tCookieManager, the cookie maps of the file system.
//	tgfsys.cpp uses them and unsorted/fn_00436000.cpp defines their constructors.
//
//	HYPOTHESIS: the original's header for these classes is not recovered; this one holds the single
//	declaration both units need.

#ifndef tCountMap_h
#define tCountMap_h

#include "Tiger/tbasic.h"

namespace std
{
	template <class _Ty> struct less;
}

// HYPOTHESIS: a hash of 256 singly linked buckets keyed by the cookie's low byte; the class,
// tMapPair, iterator, operator* and operator++(int) are named by the exe. Member names, the
// bucket layout and the inline lookup and removal bodies are not recovered. The destructor is
// virtual, as the scalar deleting destructor (STARFLEETOP 0x004361cb) calls it.
template <class tKeyType, class tValueType, class tCompare>
class tCountMap
{
public:
	tCountMap();
	virtual ~tCountMap();

	class tMapPair
	{
	public:
		tMapPair( tKeyType &key, tValueType &value );
		virtual ~tMapPair();

		tKeyType	fKey;		// +0x04
		tValueType	fValue;		// +0x0c
		int32u		fCount;		// +0x68
		tMapPair	*fNext;		// +0x6c
	};

	class iterator
	{
	public:
		iterator( tMapPair *node ) { fNode = node; }

		tMapPair	&operator*();
		iterator	operator++( int );

		tMapPair	*fNode;
	};

	iterator mFind( const tKeyType &key )
	{
		iterator	it( fHead );

		for ( ; it.fNode != 0; it++ )
			if ( (*it).fKey.fCookie == key.fCookie )
				break;
		return it;
	}

	// Out of line in the exe (STARFLEETOP 0x00621b60).
	iterator	find( tKeyType &key );
	// Out of line in the exe (STARFLEETOP 0x006220e0).
	iterator	end( void );

	tValueType	&operator[]( tKeyType &key );

	// MATCH-ONLY: operator[] as tVolume::mGetSortName (STARFLEETOP 0x0061d8b0) has it expanded
	// inline, where tVolume::mGetName calls it out of line. A found pair moves to the front of
	// its bucket; a missing key gets a new pair with a default value at the front.
	tValueType &fn_0061d8b0_index( tKeyType &key )
	{
		tMapPair	*prev = 0;
		tMapPair	*pair;

		for ( pair = fHead; pair != 0; prev = pair, pair = pair->fNext )
			if ( pair->fKey == key )
				break;

		if ( pair == 0 )
		{
			tValueType	value;

			pair = new tMapPair( key, value );
			if ( fHead != 0 )
				pair->fNext = fHead;
			else
				fTail = pair;
			fHead = pair;
		}
		else if ( prev != 0 )
		{
			prev->fNext = pair->fNext;
			pair->fNext = fHead;
			fHead = pair;
			if ( fTail == pair )
				fTail = prev;
		}
		return pair->fValue;
	}

	void mRemove( const tKeyType &key )
	{
		tMapPair	*prev = 0;
		tMapPair	*pair;

		for ( pair = fHead; pair != 0; prev = pair, pair = pair->fNext )
			if ( pair->fKey.fCookie == key.fCookie )
				break;
		if ( pair == 0 )
			throw "Couldn't find the object to delete";

		if ( --pair->fCount == 0 )
		{
			tMapPair	*head = fHead;

			if ( pair == head )
				fHead = head->fNext;
			else if ( pair == fTail )
			{
				fTail = prev;
				prev->fNext = 0;
			}
			else
				prev->fNext = pair->fNext;

			delete pair;

			if ( fHead == 0 )
				fTail = 0;
		}
	}

	tMapPair	*fHead;		// +0x04
	tMapPair	*fTail;		// +0x08
};

// HYPOTHESIS: the 256 count maps the cookie manager hashes its cookies over, by the cookie's low
// byte; the class and mResolve are named by the exe, which calls mResolve (STARFLEETOP
// 0x00637570) out of line to pick a cookie's map. The destructor is virtual, as the cookie
// manager's constructors (STARFLEETOP 0x004361e7) store its vtable at +0x00.
template <class tKeyType, class tValueType, class tCompare>
class tValueCountMap
{
public:
	tValueCountMap() {}
	virtual ~tValueCountMap();

	typedef tCountMap<tKeyType,tValueType,tCompare>	tMap;

	tMap	&mResolve( tKeyType &key );
	// Out of line in the exe (STARFLEETOP 0x00636fd0 and 0x00636fb0), named by its symbols.
	int32u		count( tKeyType &key );
	tValueType	&operator[]( tKeyType &key );

	tMap	fMaps[256];		// +0x04
};

// HYPOTHESIS: the cookie manager hashes its cookies over 256 count maps by the cookie's low byte;
// operator[] being called on one 12-byte map (STARFLEETOP 0x00621ba0) shows the maps are the
// buckets. The leading word is not recovered; the constructors (STARFLEETOP 0x004361e7 and the
// six after it) clear it.
template <class tCookieType, class tObjectType>
class tCookieManager
{
public:
	tCookieManager();

	typedef tCountMap<tCookieType,tObjectType,std::less<tCookieType> >	tMap;

	tCookieType	mMake( tObjectType &object );

	void mRemove( tCookieType &cookie )
	{
		tMap::iterator it = fValues.fMaps[cookie.fCookie & 0xff].mFind( cookie );

		if ( it.fNode == 0 || it.fNode->fCount == 0 )
			throw "Use of invalid cookie to remove an item";
		fValues.fMaps[cookie.fCookie & 0xff].mRemove( cookie );
	}

	// HYPOTHESIS: whether the cookie names a live item, as tVolume::mValid (STARFLEETOP
	// 0x0061d200) has it expanded inline; the search compares the node's key directly, where
	// mResolve's goes through the iterator. Its name is not recovered. The original frame is 4
	// bytes larger than this build's; the extra slot is not recovered.
	iTruth fn_0061d200_mValid( tCookieType &cookie )
	{
		tMap::iterator it( fValues.fMaps[cookie.fCookie & 0xff].fHead );

		for ( ; it.fNode != 0; it++ )
			if ( it.fNode->fKey.fCookie == cookie.fCookie )
				break;
		return it.fNode != 0 && it.fNode->fCount != 0;
	}

	// The search is written out here rather than through mFind: tVolume::mGetName (STARFLEETOP
	// 0x0061d7a0) keeps the iterator in this function's scope, where an inlined mFind's local
	// would share its slot with the thrown string.
	tObjectType &mResolve( tCookieType &cookie )
	{
		tMap::iterator it( fValues.fMaps[cookie.fCookie & 0xff].fHead );

		for ( ; it.fNode != 0; it++ )
			if ( (*it).fKey.fCookie == cookie.fCookie )
				break;
		if ( it.fNode == 0 || it.fNode->fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		return fValues.fMaps[cookie.fCookie & 0xff][cookie];
	}

	// MATCH-ONLY: mResolve as tVolume::mGetSortName (STARFLEETOP 0x0061d8b0) has it, with the
	// search's comparison taken as a truth value and operator[] expanded inline.
	tObjectType &fn_0061d8b0_mResolve( tCookieType &cookie )
	{
		tMap::iterator it( fValues.fMaps[cookie.fCookie & 0xff].fHead );

		for ( ; it.fNode != 0; it++ )
			if ( !( (*it).fKey != cookie ) )
				break;
		if ( it.fNode == 0 || it.fNode->fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		return fValues.fMaps[cookie.fCookie & 0xff].fn_0061d8b0_index( cookie );
	}

	// MATCH-ONLY: mResolve as tFileSession::mGetAppPath (STARFLEETOP 0x0061c070) has it, calling
	// the out-of-line tCountMap::find where tFileSession::mGetNamedVolume has the search expanded
	// inline. The original has one resolve; why the two expand differently is not recovered.
	tObjectType &fn_0061c070_mResolve( tCookieType &cookie )
	{
		tMap::iterator it = fValues.fMaps[cookie.fCookie & 0xff].find( cookie );

		if ( it.fNode == 0 || (*it).fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		return fValues.fMaps[cookie.fCookie & 0xff][cookie];
	}

	// MATCH-ONLY: mResolve as tPathIterator::mAdd (STARFLEETOP 0x0061f910) has it, searching with
	// the out-of-line tCountMap::find and testing the result against the end of the map
	// tValueCountMap::mResolve picks, which also gives operator[] its map. Why it expands
	// differently from fn_0061c070_mResolve is not recovered.
	tObjectType &fn_0061f910_mResolve( tCookieType &cookie )
	{
		tValueCountMap<tCookieType,tObjectType,std::less<tCookieType> >	&values = fValues;	tMap::iterator it = values.fMaps[cookie.fCookie & 0xff].find( cookie );
		if ( it.fNode == values.mResolve( cookie ).end().fNode || (*it).fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		tMap	&map = values.mResolve( cookie );	return map[cookie];
	}

	// MATCH-ONLY: mResolve as twDir::mMakeFullPath (STARFLEETOP 0x006315d0) has it, with the
	// bucket index held in a local for the search, the comparison taken as a truth value, the
	// found iterator copied before it is tested and operator[] out of line.
	tObjectType &fn_006315d0_mResolve( tCookieType &cookie )
	{
		int32u	index = cookie.fCookie & 0xff;
		tMap::iterator it( fValues.fMaps[index].fHead );
		for ( ; it.fNode != 0; it++ )
			if ( !( (*it).fKey != cookie ) )
				break;
		tMap::iterator found = it;
		if ( found.fNode == 0 || (*found).fCount == 0 )
			throw "Use of invalid cookie to resolve an item";
		tMap	&map = fValues.fMaps[cookie.fCookie & 0xff];
		return map[cookie];
	}

	// MATCH-ONLY: mResolve as twIORequest::mProcessRequest (STARFLEETOP 0x006340b0) has it for a
	// read, with the value map's out-of-line count and operator[].
	tObjectType &fn_006340b0_mResolve( tCookieType &cookie )
	{
		if ( fValues.count( cookie ) == 0 )
			throw "Use of invalid cookie to resolve an item";
		return fValues[cookie];
	}

	// MATCH-ONLY: the out-of-line mResolve of tCookieManager<tPath,twPath> (STARFLEETOP
	// 0x00636ad0), defined after the class so that its callers cannot inline it.
	tObjectType &fn_00636ad0_mResolve( tCookieType &cookie );

	int32u											f_00;
	tValueCountMap<tCookieType,tObjectType,std::less<tCookieType> >	fValues;	// +0x04
};

#endif
