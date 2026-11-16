// tStrMgrCore.cpp: tStrMgrCore, the string-table core behind tgStrManager.

#include "Tiger/tgStrMgr.h"
#include "Tiger/tbasicfile.h"

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

// FUNCTION: STARFLEETOP 0x00627470
const tString &tStrMgrCore::mGetRandStr( const char *group )
{
	return mGetRandStr( mGetStringIndex( group ) );
}

// FUNCTION: STARFLEETOP 0x00627490
const tString &tStrMgrCore::mGetGrpStr( long group, long element )
{
	if ( group > 0 && group <= fn_00627180_vec.size() )
	{
		if ( element > mGetNumElements( group ) )
		{
			if ( fn_00627200_throwOnMiss )
				throw (eStrManagerErr)1;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
			return gErrStr;
		}
		return mGetString( group + element );
	}
	if ( fn_00627200_throwOnMiss )
		throw (eStrManagerErr)2;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
	return gErrStr;
}

// FUNCTION: STARFLEETOP 0x00627520
const tString &tStrMgrCore::mGetGrpStr( const char *group, int element )
{
	return mGetGrpStr( mGetStringIndex( group ), element );
}

// FUNCTION: STARFLEETOP 0x00627540
unsigned char tStrMgrCore::mIsAnElement( long group, long element )
{
	if ( group > 0 && group <= fn_00627180_vec.size() )
	{
		if ( element > mGetNumElements( group ) )
		{
			if ( fn_00627200_throwOnMiss )
				throw (eStrManagerErr)1;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
			return false;
		}
		return element <= mGetNumElements( group ) ? true : false;
	}
	if ( fn_00627200_throwOnMiss )
		throw (eStrManagerErr)2;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
	return false;
}

// FUNCTION: STARFLEETOP 0x006275d0
unsigned char tStrMgrCore::mIsAnElement( const char *group, long element )
{
	return mIsAnElement( mGetStringIndex( group ), element );
}

// FUNCTION: STARFLEETOP 0x006275f0
unsigned char tStrMgrCore::mIsAnElement( long group, const tString &str )
{
	if ( group > 0 && group <= fn_00627180_vec.size() )
	{
		int count = mGetNumElements( group );
		for ( int i = 0; i < count; i++ )
		{
			if ( mGetString( group + i ) == str )
				return true;
		}
		return false;
	}
	if ( fn_00627200_throwOnMiss )
		throw (eStrManagerErr)2;	// HYPOTHESIS: the original names this value; its enumerator is not recovered
	return false;
}

// FUNCTION: STARFLEETOP 0x00627680
unsigned char tStrMgrCore::mIsAnElement( const char *group, const tString &str )
{
	return mIsAnElement( mGetStringIndex( group ), str );
}

// HYPOTHESIS: tWideCharFile (class name from the exe's symbol) reads a file of 16-bit
// characters through a tBasicFile in blocks of 0x400 characters; layout from mGetChar's
// accesses only.
class tWideCharFile
{
	public:
		unsigned short	mGetChar	( void );

		unsigned short	fn_006276a0_buffer[0x400];
		int				fn_006276a0_count;
		int				fn_006276a0_pos;
		unsigned char	fn_006276a0_swap;
		tBasicFile		*fn_006276a0_file;
};

// FUNCTION: STARFLEETOP 0x006276a0
unsigned short tWideCharFile::mGetChar( void )
{
	if ( fn_006276a0_pos >= fn_006276a0_count )
	{
		if ( fn_006276a0_count < 0 )
			return 0xffff;
		fn_006276a0_pos = 0;
		fn_006276a0_count = fn_006276a0_file->mRead( fn_006276a0_buffer, sizeof( fn_006276a0_buffer ) );
		if ( fn_006276a0_count & 1 )
			fn_006276a0_count--;
		fn_006276a0_count >>= 1;
		if ( fn_006276a0_count <= 0 )
		{
			fn_006276a0_count = -1;
			return 0xffff;
		}
		if ( fn_006276a0_swap )
		{
			// The original's byte swap: the test is reversed and the first character is
			// swapped each time, so the loop never runs for a block that was read.
			for ( int i = 0; i > fn_006276a0_count; i++ )
			{
				unsigned char *p = (unsigned char *)fn_006276a0_buffer;
				unsigned char c = p[0];
				p[0] = p[1];
				p[1] = c;
			}
		}
	}
	return fn_006276a0_buffer[fn_006276a0_pos++];
}
