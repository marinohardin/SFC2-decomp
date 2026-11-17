#include <string.h>

#include "unsorted/tSndMgr_Base.h"
#include "unsorted/ParseSFCSoundTable.h"
#include "Common/Thread/Thread.h"
#include "Tiger/tbasicfile.h"
#include "unsorted/mss.h"

class tSndMgr_Miles;

// HYPOTHESIS: a free function called before AIL_startup; its role is not known.
void fn_006f3870();

// HYPOTHESIS: the member classes below are named after their destructors until evidence names
// them; sizes come from the member offsets in tSndMgr_Miles.
// HYPOTHESIS: this is tSndZipInfoMgr (size 0x254, mInit at 0x0066c9b0), which is declared only
// inside Tiger/tbasicsoundfilezip.cpp; it stays a stand-in until a header holds that class.
class fn_0050cb19_object
{
public:
	~fn_0050cb19_object();

	unsigned char mInit( const char * );
	// HYPOTHESIS: tSndZipInfoMgr::mDone, folded in the exe with tZipInfoMgr::mDone (0x00627f60).
	void mDone();

	char pad_0000[0x254];
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

	char pad_0000[0x18];
};

class fn_0050cb59_object
{
public:
	~fn_0050cb59_object();

	// HYPOTHESIS: loads the music from a zip and its table
	void fn_0050d539( tSndMgr_Miles *param_1, const char *param_2, const char *param_3 );

	char pad_0000[0x3c8];
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
	char pad_4[0x10];
	// HYPOTHESIS: three Miles locked buffers, freed with AIL_mem_free_lock.
	void *field_14;
	char pad_18[0x8];
	void *field_20;
	void *field_24;
	char pad_28[0x18];
	long field_40;
	char pad_44[0x4];
	tBasicFile *field_48;
	tBasicFile *field_4c;
	tBasicFile *field_50;
	char pad_54[0x28];
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
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7( long param_1, long param_2 );
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19( long param_1 );
	virtual long vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28( long param_1 );
	// HYPOTHESIS: fourth parameter is a byte flag, fifth a volume-like 0..100 value
	virtual void vfunction29( long param_1, long param_2, char param_3, long param_4 );
	virtual void vfunction30();
	virtual char vfunction31();

	char pad_0004[0xc];
	// HYPOTHESIS: 32 channels, the most vfunction28 allows; the shutdown clears 0xf80 bytes.
	fn_0050eea7_sample field_10[32];
	char pad_f90[0xf90];
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
	fn_0050cb59_object field_2450;
	// HYPOTHESIS: two tThread members (destructor 0x006ff170, size 0xd0c); tThread is declared
	// only inside Common/Thread/ThreadPool.cpp, so they stay byte arrays until a header holds it.
	tThread field_2818;
	tThread field_3524;
	// HYPOTHESIS: flags shared with the callback thread (stop request, busy), hence volatile.
	volatile char field_4230;
	volatile char field_4231;
	char pad_4232[2];
	long field_4234;
	// HYPOTHESIS: two tCallback<long>-derived members (the destructor resets both to the
	// tCallback<long> vtable 0x007ab0c0), likely tMilesCallBack and tAILServeCallBack;
	// tCallback is declared only inside Common/Thread/ThreadPool.cpp.
	fn_007ab0c0_object field_4238;
	fn_007ab0c0_object field_4240;
};

// FUNCTION: STARFLEETOP 0x0050ed7a
void tSndMgr_Miles::vfunction7( long param_1, long param_2 )
{
	vfunction29( param_1, param_2, 0, 100 );
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
