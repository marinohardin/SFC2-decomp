#include <string.h>

#include "unsorted/tSndMgr_Base.h"
#include "unsorted/ParseSFCSoundTable.h"
#include "Common/Thread/Thread.h"
#include "Tiger/tbasicfile.h"
#include "Tiger/tZipInfoMgr.h"
#include "unsorted/mss.h"
#include "unsorted/tEntity.h"
#include "Source/entity/entity_physical.h"
#include "unsorted/fn_0090be9c_type.h"
#include "unsorted/fn_00c7c120_Object.h"

class tSndMgr_Miles;

// HYPOTHESIS: the object at 0x00918888, which tMyHud3D.cpp declares inside that unit; only
// slot 8 (offset 0x20) is called, returning a game state.
class tObject_00918888
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual int vfunction8();
};

extern tObject_00918888 *gObject_00918888;

// HYPOTHESIS: the camera's view and projection matrices and the left and right edges of the
// screen; fn_0050fa3c projects an entity with them to pan its sound.
extern tMatrix gMatrix_0092f5b8;
extern tMatrix gMatrix_0092f528;
extern long g_00918100;
extern long g_00918108;

// HYPOTHESIS: a free function called before AIL_startup; its role is not known.
void fn_006f3870();

// HYPOTHESIS: the member classes below are named after their destructors until evidence names
// them; sizes come from the member offsets in tSndMgr_Miles.
// HYPOTHESIS: this is tSndZipInfoMgr (size 0x254, mInit at 0x0066c9b0), which is declared only
// inside Tiger/tbasicsoundfilezip.cpp; it stays a stand-in until a header holds that class.
class fn_0050cb19_object : public tZipInfoMgr
{
public:
	~fn_0050cb19_object();

	unsigned char mInit( const char * );
	// HYPOTHESIS: mDone is inherited; the exe folds tSndZipInfoMgr::mDone with tZipInfoMgr::mDone.

	// HYPOTHESIS: the entry type is tSndZipEntry, laid out as tZipEntry.
	typedef tZipInfoMgr::tZipEntry tEntry;
	// HYPOTHESIS: tSndZipInfoMgr's lookup of a name in its directory, as tZipInfoMgr::mFindEntry.
	const tEntry *fn_0066cec0( const char * ) const;

	char pad_0110[0x254 - 0x110];
};

class fn_0050bcd4_object
{
public:
	~fn_0050bcd4_object();

	char pad_0000[0x18];
};

class fn_005d6a72_object
{
public:
	~fn_005d6a72_object();

	char pad_0000[0xc];
	// HYPOTHESIS: an id handed to tSndMgr_Miles::vfunction5 and the channel index it plays on.
	long field_c;
	long field_10;
};

class fn_0050cb59_object
{
public:
	~fn_0050cb59_object();

	// HYPOTHESIS: loads the music from a zip and its table
	void fn_0050d539( tSndMgr_Miles *param_1, const char *param_2, const char *param_3 );
	// HYPOTHESIS: looks up a music track; sets *param_2 when found.
	void fn_0050d5ec( long param_1, char *param_2 );
	// HYPOTHESIS: the current track's playback rate, sample format, type flags and ADPCM block size.
	void fn_0050d8f0( long *param_1, long *param_2, unsigned long *param_3, unsigned long *param_4 );
	// HYPOTHESIS: stops the music; vfunction22 then clears the two fields after it.
	void fn_0050d95c( char param_1 );
	// HYPOTHESIS: a value of the playing music, read by vfunction23.
	long fn_0050d7d8();

	char pad_0000[0x8];
	long field_8;
	long field_c;
	char pad_0010[0x3c8 - 0x10];
};

// HYPOTHESIS: stands in for a tCallback<long>-derived class (vtable 0x007ab0c0), likely
// tMilesCallBack or tAILServeCallBack; tCallback is declared only inside ThreadPool.cpp.
class fn_007ab0c0_object
{
public:
	virtual ~fn_007ab0c0_object() {}
	virtual void Callback( long result );
	// HYPOTHESIS: a third slot (+0x08) that reports whether the callback is still running.
	virtual char vfunction3();

	long field_4;
};

// HYPOTHESIS: one sample channel of tSndMgr_Miles (stride 0x7c from +0x10); only the Miles
// sample handle at its start is known. HYPOTHESIS: the handle is volatile (the original re-reads
// it after every store and call), as the Miles callbacks touch the channel from their thread.
struct fn_0050eea7_sample
{
	volatile HSAMPLE field_0;
	char pad_4[0x4];
	// HYPOTHESIS: the sound id and the table entry vfunction29 plays.
	long field_8;
	long field_c;
	char field_10;
	char pad_11[3];
	// HYPOTHESIS: three Miles locked buffers, freed with AIL_mem_free_lock.
	void *field_14;
	// HYPOTHESIS: the size and file offset of the data in its zip, and the play handle.
	unsigned long field_18;
	long field_1c;
	void *field_20;
	void *field_24;
	char pad_28[0x4];
	unsigned long field_2c;
	char pad_30[0xc];
	char field_3c;
	char pad_3d[0x3];
	long field_40;
	char field_44;
	char pad_45[0x3];
	tBasicFile *field_48;
	tBasicFile *field_4c;
	tBasicFile *field_50;
	// HYPOTHESIS: state flags shared with the callback thread, hence volatile.
	volatile char field_54;
	volatile char field_55;
	volatile char field_56;
	volatile char field_57;
	// HYPOTHESIS: a fade length in milliseconds, a counter and its start time (vfunction22).
	volatile long field_58;
	volatile long field_5c;
	volatile long field_60;
	// HYPOTHESIS: the channel's priority; fn_0050fb3e compares it unsigned and rereads it.
	volatile unsigned long field_64;
	char field_68;
	char pad_69[0x3];
	// HYPOTHESIS: volume (0..127), pan (64 centred) and a start offset.
	long field_6c;
	long field_70;
	long field_74;
	char field_78;
	char field_79;
	char pad_7a[0x2];
};

// HYPOTHESIS: slot layout from the analysis' vtable numbering, which counts from 1
// (vfunction29 is called through +0x70); the others keep their analysis names.
class tSndMgr_Miles : public tSndMgr_Base
{
public:
	virtual ~tSndMgr_Miles();
	virtual char vfunction2( HWND param_1 );
	virtual void vfunction3();
	virtual void vfunction4();
	// HYPOTHESIS: vfunction5 tests whether a sound id is playing, vfunction6 a general state; both
	// return a byte flag (vfunction4 tests al after each call).
	virtual char vfunction5( long param_1 );
	virtual char vfunction6();
	virtual long vfunction7( long param_1, long param_2 );
	virtual void vfunction8( long param_1 );
	virtual void vfunction9();
	// HYPOTHESIS: second parameter unused (ret 8, never read)
	virtual long vfunction10( long param_1, long param_2 );
	virtual void vfunction11( long param_1 );
	virtual void vfunction12();
	virtual void vfunction13( long param_1 );
	virtual void vfunction14( long param_1, long param_2 );
	virtual long vfunction15( long param_1, long param_2 );
	virtual void vfunction16( long param_1, long param_2, long param_3 );
	virtual void vfunction17( long param_1 );
	virtual long vfunction18();
	virtual void vfunction19( long param_1 );
	virtual long vfunction20();
	virtual void vfunction21( long param_1 );
	virtual void vfunction22( double param_1, char param_2 );
	virtual long vfunction23();
	virtual void vfunction24( long param_1 );
	virtual long vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28( long param_1 );
	// HYPOTHESIS: fourth parameter is a byte flag, fifth a volume-like 0..100 value
	virtual long vfunction29( long param_1, long param_2, char param_3, unsigned long param_4 );
	virtual void vfunction30();
	virtual char vfunction31();
	virtual void vfunction32( long param_1 );

	// HYPOTHESIS: maps a sound id to its channel index in place; returns whether it found one.
	char fn_0050fc33( long *param_1 );
	// HYPOTHESIS: helpers of vfunction29: a volume for a group and an entity id, a pan for a
	// group, a group's channel count, and two channel searches returning -1 when none is free.
	long fn_0050f9ea( long param_1, long param_2 );
	long fn_0050fa3c( long param_1 );
	unsigned long fn_0050fb04( long param_1 );
	long fn_0050fb3e( unsigned long param_1, long param_2 );
	long fn_0050fba4( long param_1, char *param_2 );

	char pad_0004[0xc];
	// HYPOTHESIS: 32 channels, the most vfunction28 allows; the shutdown clears 0xf80 bytes.
	fn_0050eea7_sample field_10[32];
	// HYPOTHESIS: a second bank of 32 channels, for sounds found in the voice zip.
	fn_0050eea7_sample field_f90[32];
	char pad_1f10[0x4];
	// HYPOTHESIS: music volume, 0..100 (vfunction21 scales it to Miles' 0..127)
	long field_1f14;
	// HYPOTHESIS: master volume, 0..100 (vfunction17 scales it to Miles' 0..127)
	long field_1f18;
	char pad_1f1c[0x4];
	long field_1f20;
	ParseSFCSoundTable field_1f24;
	fn_0050cb19_object field_1f60;
	fn_0050cb19_object field_21b4;
	tBasicFile *field_2408;
	tBasicFile *field_240c;
	unsigned long field_2410;
	char field_2414;
	char pad_2415[3];
	HDIGDRIVER field_2418;
	HWND field_241c;
	fn_0050bcd4_object field_2420;
	fn_005d6a72_object field_2438;
	// HYPOTHESIS: counts the sounds started; vfunction29 returns it as the play handle.
	long field_244c;
	fn_0050cb59_object field_2450;
	// HYPOTHESIS: two tThread members (destructor 0x006ff170, size 0xd0c); tThread is declared
	// only inside Common/Thread/ThreadPool.cpp, so they stay byte arrays until a header holds it.
	tThread field_2818;
	tThread field_3524;
	// HYPOTHESIS: flags shared with the callback thread (stop request, busy), hence volatile.
	volatile char field_4230;
	volatile char field_4231;
	char pad_4232[2];
	unsigned long field_4234;
	// HYPOTHESIS: two tCallback<long>-derived members (the destructor resets both to the
	// tCallback<long> vtable 0x007ab0c0), likely tMilesCallBack and tAILServeCallBack;
	// tCallback is declared only inside Common/Thread/ThreadPool.cpp.
	fn_007ab0c0_object field_4238;
	fn_007ab0c0_object field_4240;
};

// FUNCTION: STARFLEETOP 0x0050ed7a
long tSndMgr_Miles::vfunction7( long param_1, long param_2 )
{
	return vfunction29( param_1, param_2, 0, 100 );
}

// FUNCTION: STARFLEETOP 0x0050ed8e
void tSndMgr_Miles::vfunction19( long param_1 )
{
	field_1f20 = param_1;
}

// FUNCTION: STARFLEETOP 0x0050ed9b
long tSndMgr_Miles::vfunction20()
{
	return field_1f20;
}

// FUNCTION: STARFLEETOP 0x0050eda2
void tSndMgr_Miles::vfunction28( long param_1 )
{
	if ( param_1 > 32 )
		field_4234 = 32;
	else if ( param_1 < 8 )
		field_4234 = 8;
	else
		field_4234 = param_1;
}

// FUNCTION: STARFLEETOP 0x0050edc5
char tSndMgr_Miles::vfunction31()
{
	return field_4230;
}

// FUNCTION: STARFLEETOP 0x0050ede8
tSndMgr_Miles::~tSndMgr_Miles()
{
	vfunction3();
}

// FUNCTION: STARFLEETOP 0x0050eea7
char tSndMgr_Miles::vfunction2( HWND param_1 )
{
	// HYPOTHESIS: a static wave format, as the exe keeps it at a fixed address (0x00969330)
	static PCMWAVEFORMAT format;

	if ( !field_2414 )
	{
		field_1f60.mInit( "assets/sound/SFCSounds.zip" );
		field_21b4.mInit( "assets/sound/SFCVoice.zip" );

		ParseSFCSoundTable *table = &field_1f24;
		table->vfunction21( &field_1f60, &field_21b4 );
		table->vfunction1( "assets/sound/sounds.txt" );

		AIL_set_redist_directory( "Drivers\\" );
		fn_006f3870();
		field_2414 = AIL_startup() != 0;
		if ( field_2414 )
		{
			format.wf.wFormatTag = WAVE_FORMAT_PCM;
			format.wf.nChannels = 2;
			format.wf.nSamplesPerSec = 22050;
			format.wf.nAvgBytesPerSec = 88200;
			format.wf.nBlockAlign = 4;
			format.wBitsPerSample = 16;
			long result = AIL_waveOutOpen( &field_2418, 0, WAVE_MAPPER, (LPWAVEFORMAT)&format );
			AIL_last_error();
			field_2414 = result == 0;
			if ( field_2418 )
				AIL_set_DirectSound_HWND( field_2418, param_1 );

			unsigned long i = 0;
			while ( field_2414 && i < field_4234 )
			{
				field_10[i].field_0 = AIL_allocate_sample_handle( field_2418 );
				if ( field_10[i].field_0 )
				{
					AIL_init_sample( field_10[i].field_0 );
					AIL_set_sample_user_data( field_10[i].field_0, 0, (long)this );
					AIL_set_sample_user_data( field_10[i].field_0, 1, i );
				}
				else
					field_2414 = 0;
				i++;
			}

			if ( field_2414 )
			{
				field_2410 = AIL_minimum_sample_buffer_size( field_2418, 22050, DIG_F_STEREO_16 );
				if ( field_2410 < 0x8000 )
					field_2410 = 0x8000;

				field_2414 = 0;
				field_2408 = tBasicFile::mNewBasicFile();
				if ( field_2408 && tBasicFile::mInit( 1 ) )
					field_2414 = field_2408->mOpenForRead( "assets/sound/SFCSounds.zip" );
				field_240c = tBasicFile::mNewBasicFile();
				if ( field_240c && field_2414 && tBasicFile::mInit( 1 ) )
					field_240c->mOpenForRead( "assets/sound/SFCVoice.zip" );

				if ( field_2414 )
					field_2450.fn_0050d539( this, "Assets\\SOUND\\SFCMusic.zip", "assets\\sound\\SFCMusic.txt" );
			}
		}

		field_241c = param_1;
		// HYPOTHESIS: field_4238 is a tCallback<long>-derived member; tCallback is declared only
		// inside Common/Thread/ThreadPool.cpp, so the stand-in type is passed through a cast.
		field_2818.StartWork( (tCallback<int32> *)&field_4238, (int32)this );
		field_2818.mSetPriority( (tThread::ThreadPriority)6 );
		return field_2414;
	}
	return 1;
}

// FUNCTION: STARFLEETOP 0x0050f0f7
void tSndMgr_Miles::vfunction3()
{
	field_4230 = 1;
	while ( field_4231 )
		Sleep( 10 );
	while ( field_4240.vfunction3() )
		Sleep( 10 );

	field_1f60.mDone();
	field_21b4.mDone();

	for ( int i = 0; field_2414 && i < 32; i++ )
	{
		fn_0050eea7_sample *sample = &field_10[i];
		if ( sample->field_48 )
		{
			sample->field_48->mClose();
			sample->field_48->mDispose();
			sample->field_48 = 0;
		}
		if ( sample->field_4c )
		{
			sample->field_4c->mClose();
			sample->field_4c->mDispose();
			sample->field_4c = 0;
		}
		if ( sample->field_50 )
		{
			sample->field_50->mDispose();
			sample->field_50 = 0;
		}
		sample->field_40 = 0;
		if ( sample->field_0 )
			AIL_release_sample_handle( sample->field_0 );
		if ( sample->field_14 )
			AIL_mem_free_lock( sample->field_14 );
		if ( sample->field_20 )
			AIL_mem_free_lock( sample->field_20 );
		if ( sample->field_24 )
			AIL_mem_free_lock( sample->field_24 );
	}

	if ( field_2408 )
		field_2408->mDispose();
	if ( field_240c )
		field_240c->mDispose();
	field_2408 = 0;
	field_240c = 0;
	memset( field_10, 0, sizeof( field_10 ) );
	if ( field_2418 )
		AIL_waveOutClose( field_2418 );
	field_2418 = 0;
	AIL_shutdown();
	field_2414 = 0;
}

// FUNCTION: STARFLEETOP 0x0050f24d
void tSndMgr_Miles::vfunction4()
{
	if ( field_2414 )
	{
		long volume = 0;
		if ( vfunction6() )
			volume = AIL_sample_volume( field_10[0].field_0 );
		if ( field_2438.field_c )
		{
			if ( vfunction5( field_2438.field_c ) )
				AIL_set_sample_volume( field_10[field_2438.field_10].field_0, 0x7f - volume );
		}
	}
}

// FUNCTION: STARFLEETOP 0x0050f2a2
char tSndMgr_Miles::vfunction5( long param_1 )
{
	if ( field_2414 && param_1 && fn_0050fc33( &param_1 ) )
	{
		return field_10[param_1].field_10 || field_10[param_1].field_55 || field_10[param_1].field_56;
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0050f2ec
char tSndMgr_Miles::vfunction6()
{
	if ( !field_2414 )
	{
		return 0;
	}
	return field_10[0].field_10;
}

// FUNCTION: STARFLEETOP 0x0050f2fc
long tSndMgr_Miles::vfunction29( long param_1, long param_2, char param_3, unsigned long param_4 )
{
	if ( !field_2414 )
		return 0;

	ParseSFCSoundTable *table = &field_1f24;
	unsigned long low;
	unsigned long high;
	if ( table->vfunction17( param_1, &low, &high ) )
		param_4 = high * 127 / 100;

	unsigned long volume;
	long pan = 64;
	if ( ( param_1 < 1400 || param_1 > 5000 ) && param_2 > 0 )
	{
		// HYPOTHESIS: the original holds the entity id in the volume's local (one frame slot for
		// both; a separate local takes another slot).
		volume = -1;
		if ( gObject_00918888->vfunction8() == 3 || gObject_00918888->vfunction8() == 4 )
		{
			if ( gObject_008e1060.fn_00424946() )
			{
				volume = gObject_008e1060.fn_00424946()->vfunction12();
				if ( !gObject_008e1060.fn_lookup( volume ) )
					volume = gObject_008e1060.fn_00525081();
			}
		}
		else
			volume = gObject_008e1060.fn_00525081();

		long level = fn_0050f9ea( param_2, volume );
		if ( level < 0 )
			volume = param_4;
		else
			volume = level * param_4 / 127;
		pan = fn_0050fa3c( param_2 );
	}
	else
		volume = param_4 * 127 / 100;

	long start = 0;
	if ( table->vfunction16( param_1, &low, &high ) )
	{
		fn_0090be9c_type *source = g_0090be9c;
		double ratio = (double)source->vfunction7() / (double)source->field_20;
		start = (long)( ( high - low ) * ratio + low );
	}

	const char *name = table->vfunction11( param_1 );
	if ( name )
	{
		char fromSounds = 1;
		const fn_0050cb19_object::tEntry *entry = field_1f60.fn_0066cec0( name );
		if ( !entry )
		{
			entry = field_21b4.fn_0066cec0( name );
			fromSounds = 0;
		}
		// HYPOTHESIS: the rest sits in its own block; the frame shows the name's slot shared with
		// locals declared after it.
		if ( entry )
		{
			long priority = table->vfunction15( param_1 );
			long group = table->vfunction18( param_1 );
			char replace;
			unsigned long limit = table->vfunction19( group, &replace );
			long index;
			char second;
			if ( !limit || fn_0050fb04( group ) < limit )
				index = fn_0050fba4( priority, &second );
			else
			{
				if ( !replace )
					return 0;
				index = fn_0050fb3e( priority, group );
			}

			if ( index != -1 )
			{
				field_244c++;
				// HYPOTHESIS: volatile, as the Miles callbacks read the channel; the stores keep
				// their order.
				volatile fn_0050eea7_sample *sample;
				if ( second )
					sample = &field_f90[index];
				else
					sample = &field_10[index];
				sample->field_8 = param_1;
				sample->field_c = group;
				sample->field_18 = entry->field_8;
				sample->field_2c = entry->field_4;
				sample->field_1c = field_244c;
				if ( param_3 )
					sample->field_68 = 1;
				else
					sample->field_68 = table->vfunction13( param_1 );
				sample->field_64 = priority;
				sample->field_6c = volume;
				sample->field_70 = pan;
				sample->field_74 = start;
				sample->field_56 = 0;
				sample->field_55 = 0;
				sample->field_54 = 0;
				sample->field_57 = 0;
				sample->field_79 = 0;
				sample->field_78 = 0;
				field_10[index].field_56 = 0;
				field_10[index].field_55 = 0;
				field_10[index].field_54 = 0;
				field_10[index].field_57 = 0;
				field_10[index].field_44 = fromSounds;
				if ( second )
					field_10[index].field_56 = 1;
				else
					field_10[index].field_55 = 1;
				return field_244c;
			}
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0050f5f8
void tSndMgr_Miles::vfunction8( long param_1 )
{
	if ( field_2414 )
	{
		if ( fn_0050fc33( &param_1 ) )
			field_10[param_1].field_54 = 1;
		else
		{
			for ( unsigned long i = 1; i < field_4234; i++ )
			{
				if ( field_10[i].field_56 && field_f90[i].field_8 == param_1 )
				{
					field_10[i].field_56 = 0;
					field_10[i].field_54 = 1;
					return;
				}
			}
			if ( fn_0050fc33( &param_1 ) )
				field_10[param_1].field_54 = 1;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0050f66f
void tSndMgr_Miles::vfunction9()
{
	for ( unsigned short i = 1; i < field_4234; i++ )
		field_10[i].field_54 = 1;
}

// FUNCTION: STARFLEETOP 0x0050f691
long tSndMgr_Miles::vfunction10( long param_1, long param_2 )
{
	if ( !field_2414 )
		return 0;
	return vfunction29( param_1, -1, 0, field_1f20 );
}

// FUNCTION: STARFLEETOP 0x0050f6b4
void tSndMgr_Miles::vfunction11( long param_1 )
{
	if ( field_2414 )
	{
		vfunction12();
		field_2438.field_10 = field_2438.field_c = vfunction29( param_1, -1, 1, 10 );
		fn_0050fc33( &field_2438.field_10 );
	}
}

// FUNCTION: STARFLEETOP 0x0050f6f0
void tSndMgr_Miles::vfunction12()
{
	if ( field_2414 )
	{
		if ( field_2438.field_c )
			vfunction8( field_2438.field_c );
		field_2438.field_c = 0;
		field_2438.field_10 = -1;
	}
}

// FUNCTION: STARFLEETOP 0x0050f720
void tSndMgr_Miles::vfunction13( long param_1 )
{
	vfunction7( 0x2c, param_1 );
	vfunction7( 0x2d, param_1 );
}

// FUNCTION: STARFLEETOP 0x0050f73f
void tSndMgr_Miles::vfunction14( long param_1, long param_2 )
{
	vfunction7( 0x2e, param_2 );
	vfunction8( param_1 );
}

// FUNCTION: STARFLEETOP 0x0050f75c
long tSndMgr_Miles::vfunction15( long param_1, long param_2 )
{
	switch ( param_1 )
	{
	case 0:
		vfunction7( 10, param_2 );
		return vfunction7( 0xb, param_2 );
	case 1:
		vfunction7( 0xd, param_2 );
		return vfunction7( 0xe, param_2 );
	case 2:
		vfunction7( 0x10, param_2 );
		return vfunction7( 0x11, param_2 );
	case 3:
		vfunction7( 0x13, param_2 );
		return vfunction7( 0x14, param_2 );
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0050f7d3
void tSndMgr_Miles::vfunction16( long param_1, long param_2, long param_3 )
{
	vfunction8( param_1 );
	switch ( param_2 )
	{
	case 0:
		vfunction7( 0xc, param_3 );
		break;
	case 1:
		vfunction7( 0xf, param_3 );
		break;
	case 2:
		vfunction7( 0x12, param_3 );
		break;
	case 3:
		vfunction7( 0x15, param_3 );
		break;
	}
}

// FUNCTION: STARFLEETOP 0x0050f81e
void tSndMgr_Miles::vfunction17( long param_1 )
{
	field_1f18 = param_1;
	if ( field_2414 )
		AIL_set_digital_master_volume( field_2418, param_1 * 127 / 100 );
}

// FUNCTION: STARFLEETOP 0x0050f84c
long tSndMgr_Miles::vfunction18()
{
	return field_1f18;
}

// FUNCTION: STARFLEETOP 0x0050f853
void tSndMgr_Miles::vfunction21( long param_1 )
{
	if ( field_2414 )
	{
		char found = 0;
		field_2450.fn_0050d5ec( param_1, &found );
		if ( found || !field_10[0].field_10 )
		{
			AIL_init_sample( field_10[0].field_0 );
			vfunction32( 0 );
			long rate;
			unsigned long flags;
			unsigned long blockSize;
			field_2450.fn_0050d8f0( &rate, &param_1, &flags, &blockSize );
			AIL_set_sample_playback_rate( field_10[0].field_0, rate );
			AIL_set_sample_type( field_10[0].field_0, param_1, flags );
			AIL_set_sample_volume( field_10[0].field_0, field_1f14 * 127 / 100 );
			if ( param_1 == 7 || param_1 == 5 )
				AIL_set_sample_adpcm_block_size( field_10[0].field_0, blockSize );
			field_10[0].field_56 = 0;
			field_10[0].field_54 = 0;
			field_10[0].field_57 = 0;
			field_10[0].field_64 = 1000;
			field_10[0].field_78 = 1;
			field_10[0].field_10 = 1;
			field_10[0].field_3c = 1;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0050f92b
void tSndMgr_Miles::vfunction22( double param_1, char param_2 )
{
	if ( field_2414 )
	{
		if ( param_2 )
		{
			fn_0050cb59_object *music = &field_2450;
			music->fn_0050d95c( 1 );
			music->field_8 = 0;
			music->field_c = 0;
			field_10[0].field_54 = 1;
		}
		else if ( param_1 < 0.1 )
			field_2450.fn_0050d95c( 1 );
		else if ( !field_10[0].field_57 )
		{
			field_10[0].field_58 = param_1 * 1000.0;
			field_10[0].field_5c = 0;
			field_10[0].field_60 = g_00c7c120.fn_006a0d50();
			field_10[0].field_57 = 1;
		}
	}
}

// FUNCTION: STARFLEETOP 0x0050f9af
long tSndMgr_Miles::vfunction23()
{
	if ( field_2414 && vfunction6() )
		return field_2450.fn_0050d7d8();
	return -1;
}

// FUNCTION: STARFLEETOP 0x0050f9d6
void tSndMgr_Miles::vfunction24( long param_1 )
{
	field_1f14 = param_1;
}

// FUNCTION: STARFLEETOP 0x0050f9e3
long tSndMgr_Miles::vfunction25()
{
	return field_1f14;
}

// FUNCTION: STARFLEETOP 0x0050f9ea
long tSndMgr_Miles::fn_0050f9ea( long param_1, long param_2 )
{
	float distance = gObject_008e1060.fn_004246ab( param_1, param_2 );
	if ( distance < 0.0f )
		return -1;
	// HYPOTHESIS: a fast float-to-int round: adding 1.5 * 2^23 leaves the rounded value in
	// the low bits of the float.
	float round = 127.0f - distance * 0.2857143f + 12582912.0f;
	long volume = *(short *)&round;
	if ( volume < 0 )
	{
		// MATCH-ONLY: an if/else assigning 0 or 1 folds into setge; the original branches.
		if ( volume < -50 )
			return 0;
		volume = 1;
	}
	return volume;
}

// FUNCTION: STARFLEETOP 0x0050fa3c
long tSndMgr_Miles::fn_0050fa3c( long param_1 )
{
	tPhysicalEntity *entity = gObject_008e1060.fn_00424a24( gObject_008e1060.fn_lookup( param_1 ) );
	if ( !entity )
		return 0;
	tPoint3 position = entity->vfunction29( tPoint3( 0.0f, 0.0f, 0.0f ), true );
	gMatrix_0092f5b8.mTransform( position );
	tPoint4 screen;
	gMatrix_0092f528.mTransform( screen, position );
	long center = ( g_00918100 + g_00918108 ) >> 1;
	long half = g_00918108 - center;
	if ( half == 0 )
		return 0;
	long pan = ( 1.0f / screen.w * screen.x - center ) * 100.0f / half;
	if ( pan > 100 )
		pan = 100;
	if ( pan < -100 )
		pan = -100;
	return ( pan * 127 + 12701 ) / 200;
}

// FUNCTION: STARFLEETOP 0x0050fb04
unsigned long tSndMgr_Miles::fn_0050fb04( long param_1 )
{
	unsigned long count = 0;
	for ( unsigned long i = 1; i < field_4234; i++ )
	{
		if ( ( field_10[i].field_10 || field_10[i].field_55 || field_10[i].field_56 ) && field_10[i].field_c == param_1 )
			count++;
	}
	return count;
}

// FUNCTION: STARFLEETOP 0x0050fb3e
long tSndMgr_Miles::fn_0050fb3e( unsigned long param_1, long param_2 )
{
	long found = -1;
	for ( unsigned long i = 0; i < field_4234; i++ )
	{
		if ( field_10[i].field_c == param_2 && field_10[i].field_64 <= param_1 && !field_10[i].field_56 && ( found == -1 || field_10[i].field_64 <= field_10[found].field_64 ) )
			found = i;
	}
	return found;
}

// FUNCTION: STARFLEETOP 0x0050fba4
long tSndMgr_Miles::fn_0050fba4( long param_1, char *param_2 )
{
	unsigned long i;
	for ( i = 1; i < field_4234; i++ )
	{
		if ( !field_10[i].field_10 && !field_10[i].field_3c && !field_10[i].field_55 && !field_10[i].field_56 )
		{
			*param_2 = 0;
			return i;
		}
	}
	long found = -1;
	for ( i = 1; i < field_4234; i++ )
	{
		if ( field_10[i].field_64 < param_1 && !field_10[i].field_55 && !field_10[i].field_56 )
		{
			if ( found == -1 )
				*param_2 = 1;
			else if ( field_10[i].field_64 >= field_10[found].field_64 )
				continue;
			found = i;
		}
	}
	return found;
}
