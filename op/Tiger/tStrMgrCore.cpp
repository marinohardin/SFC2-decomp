// tStrMgrCore.cpp: tStrMgrCore, the string-table core behind tgStrManager.

#include "Tiger/tgStrMgr.h"

// Name from the SDK library symbol at 0x00c3b650; a failed lookup returns it.
tString gErrStr;

// FUNCTION: STARFLEETOP 0x00627180
void tStrMgrCore::mDone( void )
{
	fn_00627180_map1.clear();
	// HYPOTHESIS: the member is a map<long, long> (mGetNumElements' _Lbound is that
	// instantiation), but this erase is the map<int, tString> one.
	// MATCH-ONLY: view the member as the type whose erase the exe calls.
	( (std::map<int, tString> *)&fn_00627180_map2 )->clear();
	for ( std::vector<tString *>::iterator it = fn_00627180_vec.begin(); it != fn_00627180_vec.end(); ++it )
	{
		delete *it;
	}
	fn_00627180_vec.clear();
	fn_00627180_count = 0;
}

// FUNCTION: STARFLEETOP 0x00627200
int tStrMgrCore::mGetStringIndex( const tString &s )
{
	std::map<tString, int, less_ss>::iterator it = fn_00627180_map1.find( s );
	if ( it != fn_00627180_map1.end() )
		return (*it).second;
	if ( fn_00627200_throwOnMiss )
		throw (eStrManagerErr)0;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
	return 0;
}

// FUNCTION: STARFLEETOP 0x00627280
int tStrMgrCore::mGetStringIndex( const char *s )
{
	return mGetStringIndex( tString( s ) );
}

// FUNCTION: STARFLEETOP 0x006272e0
const tString &tStrMgrCore::mGetString( int index )
{
	if ( index > 0 && index <= fn_00627180_vec.size() )
		return *fn_00627180_vec[index - 1];
	if ( fn_00627200_throwOnMiss )
		throw (eStrManagerErr)2;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
	return gErrStr;
}

// FUNCTION: STARFLEETOP 0x00627340
const tString &tStrMgrCore::mGetString( const char *s )
{
	return mGetString( mGetStringIndex( s ) );
}

// FUNCTION: STARFLEETOP 0x00627360
const tString &tStrMgrCore::mGetString( const tString &s )
{
	return mGetString( mGetStringIndex( s ) );
}

// FUNCTION: STARFLEETOP 0x00627380
long tStrMgrCore::mGetNumElements( long group )
{
	std::map<long, long>::iterator it = fn_00627180_map2.find( group );
	if ( it != fn_00627180_map2.end() )
		return (*it).second;
	return 0;
}

// FUNCTION: STARFLEETOP 0x006273e0
long tStrMgrCore::mGetNumElements( const char *group )
{
	return mGetNumElements( mGetStringIndex( group ) );
}

// FUNCTION: STARFLEETOP 0x00627400
const tString &tStrMgrCore::mGetRandStr( long group )
{
	if ( group > 0 && group <= fn_00627180_vec.size() )
	{
		long count = mGetNumElements( group );
		long index = group + rand() % count;
		return mGetString( index );
	}
	if ( fn_00627200_throwOnMiss )
		throw (eStrManagerErr)2;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
	return gErrStr;
}
