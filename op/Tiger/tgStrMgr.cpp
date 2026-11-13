// tgStrMgr.cpp: tgStrManager, the game's string-table manager (unit named from the SDK).

#include "Tiger/tgStrMgr.h"

// FUNCTION: STARFLEETOP 0x0066be30
void tgStrManager::mDeactivate( void )
{
	mDone();
}

// FUNCTION: STARFLEETOP 0x0066be50
void tgStrManager::mDone( void )
{
	fn_0066be50_core.mDone();
}

// FUNCTION: STARFLEETOP 0x0066be60
unsigned char tgStrManager::mLoadStringFile( const char *pFileName, bool bFlag )
{
	return fn_0066be50_core.mLoadStringFile( pFileName, bFlag );
}

// FUNCTION: STARFLEETOP 0x0066be80
unsigned char tgStrManager::mLoadStringFile( tBasicFile &file, bool bFlag )
{
	return fn_0066be50_core.mLoadStringFile( file, bFlag );
}

// FUNCTION: STARFLEETOP 0x0066bea0
int tgStrManager::mGetStringIndex( const char *pString )
{
	return fn_0066be50_core.mGetStringIndex( pString );
}

// FUNCTION: STARFLEETOP 0x0066bec0
int tgStrManager::mGetStringIndex( const tString &string )
{
	return fn_0066be50_core.mGetStringIndex( string );
}

// FUNCTION: STARFLEETOP 0x0066bee0
const tString &tgStrManager::mGetString( int index )
{
	return fn_0066be50_core.mGetString( index );
}

// FUNCTION: STARFLEETOP 0x0066bf00
const tString &tgStrManager::mGetString( const char *pString )
{
	return fn_0066be50_core.mGetString( pString );
}

// FUNCTION: STARFLEETOP 0x0066bf20
const tString &tgStrManager::mGetString( const tString &string )
{
	return fn_0066be50_core.mGetString( string );
}

// FUNCTION: STARFLEETOP 0x0066bf40
long tgStrManager::mGetNumElements( long n )
{
	return fn_0066be50_core.mGetNumElements( n );
}

// FUNCTION: STARFLEETOP 0x0066bf60
long tgStrManager::mGetNumElements( const char *pString )
{
	return fn_0066be50_core.mGetNumElements( pString );
}

// FUNCTION: STARFLEETOP 0x0066bf80
const tString &tgStrManager::mGetRandStr( long n )
{
	return fn_0066be50_core.mGetRandStr( n );
}

// FUNCTION: STARFLEETOP 0x0066bfa0
const tString &tgStrManager::mGetRandStr( const char *pString )
{
	return fn_0066be50_core.mGetRandStr( pString );
}

// FUNCTION: STARFLEETOP 0x0066bfc0
const tString &tgStrManager::mGetGrpStr( long group, long n )
{
	return fn_0066be50_core.mGetGrpStr( group, n );
}

// FUNCTION: STARFLEETOP 0x0066bfe0
const tString &tgStrManager::mGetGrpStr( const char *pString, int n )
{
	return fn_0066be50_core.mGetGrpStr( pString, n );
}

// FUNCTION: STARFLEETOP 0x0066c000
unsigned char tgStrManager::mIsAnElement( long group, long n )
{
	return fn_0066be50_core.mIsAnElement( group, n );
}

// FUNCTION: STARFLEETOP 0x0066c020
unsigned char tgStrManager::mIsAnElement( const char *pString, long n )
{
	return fn_0066be50_core.mIsAnElement( pString, n );
}

// FUNCTION: STARFLEETOP 0x0066c040
unsigned char tgStrManager::mIsAnElement( long group, const tString &string )
{
	return fn_0066be50_core.mIsAnElement( group, string );
}

// FUNCTION: STARFLEETOP 0x0066c060
unsigned char tgStrManager::mIsAnElement( const char *pString, const tString &string )
{
	return fn_0066be50_core.mIsAnElement( pString, string );
}
