#if !defined( mss_h )
#define mss_h

#include <windows.h>
#include <mmsystem.h>

// HYPOTHESIS: the part of the Miles Sound System interface the game calls, declared from the
// imports of mss32.dll (stdcall, decorated by their argument bytes); the handle types are
// opaque pointers as far as the game is concerned.
typedef struct _DIG_DRIVER	*HDIGDRIVER;
typedef struct _SAMPLE		*HSAMPLE;

extern "C"
{
__declspec( dllimport ) char *		__stdcall AIL_set_redist_directory( const char *dir );
__declspec( dllimport ) long		__stdcall AIL_startup( void );
__declspec( dllimport ) long		__stdcall AIL_waveOutOpen( HDIGDRIVER *drvr, LPHWAVEOUT *lphWaveOut, long wDeviceID, LPWAVEFORMAT lpFormat );
__declspec( dllimport ) char *		__stdcall AIL_last_error( void );
__declspec( dllimport ) long		__stdcall AIL_set_DirectSound_HWND( HDIGDRIVER dig, HWND wnd );
__declspec( dllimport ) HSAMPLE		__stdcall AIL_allocate_sample_handle( HDIGDRIVER dig );
__declspec( dllimport ) void		__stdcall AIL_init_sample( HSAMPLE S );
__declspec( dllimport ) void		__stdcall AIL_set_sample_user_data( HSAMPLE S, unsigned long index, long value );
__declspec( dllimport ) long		__stdcall AIL_minimum_sample_buffer_size( HDIGDRIVER dig, long playback_rate, long format );
__declspec( dllimport ) void		__stdcall AIL_release_sample_handle( HSAMPLE S );
__declspec( dllimport ) void		__stdcall AIL_mem_free_lock( void *ptr );
__declspec( dllimport ) void		__stdcall AIL_waveOutClose( HDIGDRIVER drvr );
__declspec( dllimport ) void		__stdcall AIL_shutdown( void );
__declspec( dllimport ) long		__stdcall AIL_sample_volume( HSAMPLE S );
__declspec( dllimport ) void		__stdcall AIL_set_sample_volume( HSAMPLE S, long volume );
}

#define DIG_F_STEREO_16		3

#endif
