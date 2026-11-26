#include "Tiger/tvectormath.h"
#include "unsorted/sfcstreams.h"
#include "unsorted/tEntity_Seeker.h"

void fn_0040e689(tSFCStream &s);
void fn_0040e691(tSFCStream &s);
void fn_0040ed22(tSFCStream &s, unsigned char flags);
void fn_0040ecfd(char *text, tSFCStream &s, unsigned char flags);
void fn_0040e9df(long *value, tSFCStream &s, unsigned char flags, char *name);
void fn_0040eafc(unsigned char *value, tSFCStream &s, unsigned char flags, char *name);
void fn_0040eb35(float *value, tSFCStream &s, unsigned char flags, char *name);
void fn_0040eb6e(tPoint3 *value, tSFCStream &s, unsigned char flags, char *name);

// HYPOTHESIS: the member layout from +0x248 on, as the stream calls address it.
// FUNCTION: STARFLEETOP 0x0041e64f
void tEntity_Seeker::vfunction11(tSFCStream &s, int32u mode)
{
	tEntity_Weapon::vfunction11(s, mode);
	fn_0040ed22(s, mode);
	fn_0040ecfd("Entity seeker:", s, mode);
	fn_0040e689(s);
	fn_0040eb6e((tPoint3 *)&field_0x248[0], s, mode, "fDirection");
	fn_0040eb6e((tPoint3 *)&field_0x248[0xc], s, mode, "fVelocity");
	fn_0040eb35(&field_0x260, s, mode, "fSpeed");
	fn_0040eb35(&field_0x264, s, mode, "fScale");
	fn_0040e9df((long *)&field_0x268, s, mode, "fHealth");
	fn_0040e9df((long *)&field_0x270, s, mode, "fMaxHealth");
	fn_0040e9df((long *)&field_0x26c, s, mode, "fDamageAccrued");
	fn_0040eafc((unsigned char *)&field_0x274, s, mode, "fExplosionMode");
	fn_0040eafc((unsigned char *)&field_0x275, s, mode, "fActive");
	fn_0040e9df((long *)&field_0x278, s, mode, "fLastWeasel");
	fn_0040e691(s);
}
