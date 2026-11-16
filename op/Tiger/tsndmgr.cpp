//	tsndmgr.cpp: the sound file manager and its play information.

#include "Tiger/tbasic.h"
#include "Tiger/tgSound.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// HYPOTHESIS: fields as mInit stores them; field names and the virtuals' slot order are not recovered.
class tAFPlayInfo
{
public:
	virtual	long	mGetRandomFrequency	( void );
	virtual	long	mGetRandomVolume	( void );
	virtual	long	mGetAcualIndexOffsetIfRandom	( void );
	virtual	void	mSetNextInSequence	( long next );
	virtual	iTruth	mAddClass	( long theClass );
	virtual	void	mInit	( long, char *, char *, char *, long, long, long, long, double, double, unsigned long, unsigned long, unsigned char, unsigned char, long );
	virtual	unsigned char	mIsThisInThisThreshold	( long value );
	virtual	unsigned char	mIsThisInThisGroup	( long value );

	tAFPlayInfo &	operator=	( tAFPlayInfo &other );

	// HYPOTHESIS: 0x4 is the alignment gap VC6 leaves after the vtable pointer for the doubles.
	long			field_0x8;
	char			field_0xc[0x20];
	char			field_0x2c[0x20];
	char			field_0x4c[0x40];
	long			field_0x8c;
	long			field_0x90;
	long			field_0x94;
	long			field_0x98;
	char			fUnknown_009c[0x4];
	double			field_0xa0;
	double			field_0xa8;
	long			field_0xb0;
	unsigned long	field_0xb4;
	unsigned long	field_0xb8;
	long			field_0xbc;
	unsigned char	field_0xc0;
	unsigned char	field_0xc1;
	char			fUnknown_00c2[0x2];
	// HYPOTHESIS: a list of up to four classes at 0xc4 and its count at 0xd4, from mAddClass.
	long			field_0xc4[4];
	long			field_0xd4;
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

// FUNCTION: STARFLEETOP 0x00663850
long	tAFPlayInfo::mGetRandomVolume( void )
{
	if ( field_0x94 == field_0x98 )
		return field_0x94;
	return field_0x94 - (long)( (double)rand() * ( field_0x98 - field_0x94 ) * ( -1.0 / RAND_MAX ) );
}

// FUNCTION: STARFLEETOP 0x006638a0
long	tAFPlayInfo::mGetAcualIndexOffsetIfRandom( void )
{
	if ( field_0xb0 < 2 )
		return 0;
	return ( rand() * field_0xb0 ) / RAND_MAX;
}

// FUNCTION: STARFLEETOP 0x006638e0
iTruth	tAFPlayInfo::mAddClass( long theClass )
{
	for ( long i = 0; i < field_0xd4; i++ )
	{
		if ( field_0xc4[i] == theClass )
			return kTrue;
	}
	if ( field_0xd4 == 4 )
	{
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt("We've hit the class limit!"); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt("We've hit the class limit!"); }
#line 254
		if (atgSoundPart->mAreWeInInterruptMode() == kTrue) { AddSoundErrorDuringInterrupt(" -- %s %d\n", __FILE__, __LINE__); } else { PrintOutBufferedErrors(); gSoundSay.mJustSayIt(" -- %s %d\n", __FILE__, __LINE__); }
#line 128
		return kFalse;
	}
	field_0xc4[field_0xd4] = theClass;
	field_0xd4++;
	return kTrue;
}

// FUNCTION: STARFLEETOP 0x006639c0
void	tAFPlayInfo::mSetNextInSequence( long next )
{
	field_0xbc = next;
}

// FUNCTION: STARFLEETOP 0x006639d0
void	tAFPlayInfo::mInit( long a1, char *s1, char *s2, char *s3, long a5, long a6, long a7, long a8, double d1, double d2, unsigned long u1, unsigned long u2, unsigned char b1, unsigned char b2, long next )
{
	field_0x8 = a1;
	strcpy( field_0xc, s1 );
	strcpy( field_0x2c, s2 );
	strcpy( field_0x4c, s3 );
	field_0x8c = a5;
	field_0x90 = a6;
	field_0xbc = next;
	field_0x94 = a7;
	field_0x98 = a8;
	field_0xc0 = b1;
	field_0xc1 = b2;
	field_0xd4 = 0;
	field_0xb0 = 0;
	field_0xa0 = d1;
	field_0xa8 = d2;
	field_0xb4 = u1;
	field_0xb8 = u2;
}

// FUNCTION: STARFLEETOP 0x00663ae0
tAFPlayInfo &	tAFPlayInfo::operator=( tAFPlayInfo &other )
{
	field_0x8 = other.field_0x8;
	strcpy( field_0xc, other.field_0xc );
	strcpy( field_0x2c, other.field_0x2c );
	strcpy( field_0x4c, other.field_0x4c );
	field_0x8c = other.field_0x8c;
	field_0x90 = other.field_0x90;
	field_0x94 = other.field_0x94;
	field_0x98 = other.field_0x98;
	field_0xc0 = other.field_0xc0;
	field_0xc1 = other.field_0xc1;
	field_0xd4 = other.field_0xd4;
	for ( long i = 0; i < field_0xd4; i++ )
		field_0xc4[i] = other.field_0xc4[i];
	field_0xa0 = other.field_0xa0;
	field_0xa8 = other.field_0xa8;
	field_0xb0 = other.field_0xb0;
	field_0xb4 = other.field_0xb4;
	field_0xb8 = other.field_0xb8;
	field_0xbc = other.field_0xbc;
	return *this;
}

// FUNCTION: STARFLEETOP 0x00663c40
unsigned char	tAFPlayInfo::mIsThisInThisThreshold( long value )
{
	if ( field_0x2c[0] == 0 || value == -1 )
		return 1;
	long	wanted = value;
	long	pos = 0;
	long	found;
	if ( sscanf( field_0x2c, "%ld", &found ) <= 0 || wanted == found || found == -1 )
		return 1;
	while ( field_0x2c[pos] != 0 )
	{
		while ( field_0x2c[pos] != ',' && field_0x2c[pos] != ';' && field_0x2c[pos] != 0 )
			pos++;
		if ( field_0x2c[pos] != 0 )
			pos++;
		// HYPOTHESIS: the original scans from the start of the string each time, not from pos.
		if ( sscanf( field_0x2c, "%ld", &found ) <= 0 )
			break;
		if ( wanted == found || found == -1 )
			return 1;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x00663ce0
unsigned char	tAFPlayInfo::mIsThisInThisGroup( long value )
{
	if ( field_0xc[0] == 0 )
		return 1;
	long	wanted = value;
	long	pos = 0;
	long	found;
	if ( wanted == 0 || sscanf( field_0xc, "%ld", &found ) <= 0 || wanted == found || found == 0 )
		return 1;
	do
	{
		char	c = field_0xc[pos];
		if ( c == 0 )
			return 0;
		while ( c != ',' && c != ';' && c != 0 )
			c = field_0xc[++pos];
		if ( field_0xc[pos] != 0 )
			pos++;
		if ( sscanf( &field_0xc[pos], "%ld", &found ) <= 0 )
			return 0;
	} while ( wanted != found && found != 0 );
	return 1;
}

#include "Tiger/tpart.h"
#include "unsorted/tRegisteredAudioType.h"

// HYPOTHESIS: declared only as far as the functions below need.
class tBasicSoundFileManager : public tPart
{
public:
	tBasicSoundFileManager( void );
	virtual	~tBasicSoundFileManager( void );
	virtual	unsigned long	mGetTrackingIndex	( unsigned long handle );

	// HYPOTHESIS: fields placed by the offsets the constructor stores; names not recovered.
	long					field_0xc;
	long					field_0x10;
	long					field_0x14;
	tRegisteredAudioType	field_0x18[0x40];
	long					field_0x718;
	long					field_0x71c;
	long					field_0x720;
	long					field_0x724;
	char					fUnknown_0728[0x8];
	double					field_0x730;
	char					fUnknown_0738[0x8];
	double					field_0x740;
	long					field_0x748;
	char					fUnknown_074c[0x400];
	long					field_0xb4c[0x100];
	char					fUnknown_0f4c[0x1400];
	long					field_0x234c;
	short					field_0x2350[0x100];
	char					fUnknown_2550[0x28];
	unsigned char			field_0x2578;
};

// FUNCTION: STARFLEETOP 0x00663d80
tBasicSoundFileManager::tBasicSoundFileManager( void )
{
	field_0x71c = 0;
	field_0x720 = 0;
	field_0x724 = 0;
	field_0xc = 0;
	field_0x10 = 0;
	field_0x14 = 0;
	field_0x718 = 0;
	field_0x748 = 0;
	field_0x730 = 1.0;
	field_0x2578 = 0;
	for ( int i = 0; i < 0x100; i++ )
	{
		field_0xb4c[i] = -1;
		field_0x2350[i] = 1;
	}
	field_0x234c = 0;
	field_0x740 = 2000.0;
}

// HYPOTHESIS: empty body; the destructor's unwind frame fits the compiler's destruction of
// field_0x18 and the tPart base alone.
// FUNCTION: STARFLEETOP 0x00663e60
// FUNCTION: STARFLEETOP 0x00663e80
tBasicSoundFileManager::~tBasicSoundFileManager( void )
{
}

// FUNCTION: STARFLEETOP 0x00664870
unsigned long	tBasicSoundFileManager::mGetTrackingIndex( unsigned long handle )
{
	return handle & 0xffff;
}
