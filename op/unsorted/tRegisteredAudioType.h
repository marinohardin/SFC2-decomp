//	tRegisteredAudioType.h: one registered audio type, 0x1c bytes (the sound file manager keeps 0x40 of them).

#ifndef	tRegisteredAudioType_h
#define	tRegisteredAudioType_h	1

// HYPOTHESIS: only the size is known, from the sound file manager's array stride.
class tRegisteredAudioType
{
	public:
						tRegisteredAudioType	( void );
						~tRegisteredAudioType	( void );

		char			fUnknown_0000[0x1c];
};

#endif
