//	tsndmgr.cpp: the sound file manager and its play information.

#include "Tiger/tbasic.h"
#include <string.h>
#include <stdlib.h>

// HYPOTHESIS: declared only as far as the functions below need: mGetRandomFrequency reads a
// pair of frequency bounds at 0x8c and 0x90, mSetNextInSequence stores 0xbc; the other fields
// and the virtuals' slot order are not recovered.
class tAFPlayInfo
{
public:
	virtual	long	mGetRandomFrequency	( void );
	virtual	void	mSetNextInSequence	( long next );

	char			fUnknown_0004[0x88];
	long			field_0x8c;
	long			field_0x90;
	char			fUnknown_0094[0x28];
	long			field_0xbc;
};

// HYPOTHESIS: declared only as far as the functions below need.
class tBasicSoundFileManager
{
public:
	virtual	unsigned long	mGetTrackingIndex	( unsigned long handle );
};

// FUNCTION: STARFLEETOP 0x00663720
unsigned char	mystrequivalent( char *s1, char *s2 )
{
	int		len = strlen( s1 );
	if ( len != (int)strlen( s2 ) )
		return 0;
	for ( int i = 0; i < len; i++ )
	{
		char	c1 = s1[i];
		char	c2 = s2[i];
		if ( c1 >= 'a' )
			c1 -= 0x20;
		if ( c2 >= 'a' )
			c2 -= 0x20;
		if ( c1 != c2 )
			return 0;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x00663780
unsigned char	mystrequivalentN( char *s1, char *s2, long n )
{
	int		len1 = strlen( s1 );
	int		len2 = strlen( s2 );
	if ( len1 > n )
		len1 = n;
	if ( len2 > n )
		len2 = n;
	if ( len1 != len2 )
		return 0;
	for ( int i = 0; i < len1; i++ )
	{
		char	c1 = s1[i];
		char	c2 = s2[i];
		if ( c1 >= 'a' )
			c1 -= 0x20;
		if ( c2 >= 'a' )
			c2 -= 0x20;
		if ( c1 != c2 )
			return 0;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x006637f0
long	tAFPlayInfo::mGetRandomFrequency( void )
{
	if ( field_0x8c == field_0x90 )
		return -1;
	return field_0x8c - (long)( (double)rand() * ( field_0x90 - field_0x8c ) * ( -1.0 / RAND_MAX ) );
}

// FUNCTION: STARFLEETOP 0x006639c0
void	tAFPlayInfo::mSetNextInSequence( long next )
{
	field_0xbc = next;
}

// FUNCTION: STARFLEETOP 0x00664870
unsigned long	tBasicSoundFileManager::mGetTrackingIndex( unsigned long handle )
{
	return handle & 0xffff;
}
