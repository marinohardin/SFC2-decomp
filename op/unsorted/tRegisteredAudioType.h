//	tRegisteredAudioType.h: one registered audio type, 0x1c bytes (the sound file manager keeps 0x40 of them).

#ifndef	tRegisteredAudioType_h
#define	tRegisteredAudioType_h	1

class tSoundChannelFormat;

// HYPOTHESIS: the size is the sound file manager's array stride; the fields are placed by the
// stores of tBasicSoundFileManager::mAddRegisteredAudioType, their names not recovered.
class tRegisteredAudioType
{
	public:
						tRegisteredAudioType	( void );
						~tRegisteredAudioType	( void );

		char *			field_0x0;
		unsigned long	(*field_0x4)( char *, long, long, long, tSoundChannelFormat *, long, void * );
		long			field_0x8;
		long			field_0xc;
		void *			field_0x10;
		unsigned long	(*field_0x14)( unsigned long, long );
		long			field_0x18;
};

#endif
