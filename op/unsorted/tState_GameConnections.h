//	tState_GameConnections.h: tState_Namespace_GameConnections::tState_GameConnections, the
//	game connections state.
// Evidence:  class (RTTI; original file unknown). vftable 0x0079ebec, constructor 0x0046981c
//            (the one object is a global at 0x00845830, so no allocation gives the size; the
//            highest member the code uses is the int at 0x23c).

#ifndef tState_GameConnections_h
#define tState_GameConnections_h 1

#include <vector>
#include "Tiger/tstring.h"
#include "unsorted/tValueI.h"
#include "unsorted/tGameState.h"
#include "unsorted/c_0046aa10_msg.h"
#include "unsorted/c_00468677_source.h"

namespace tState_Namespace_GameConnections
{

// HYPOTHESIS: only the slots rebuilt so far; members placed by their offsets.
class tState_GameConnections : public tGameState
{
public:
	tState_GameConnections();
	// No destructor declared: the exe's (0x004696e0) destroys the members and calls ~tGameState
	// without storing the vtable first, as only VC6's implicit destructor does. Slot 0 holds the
	// scalar deleting destructor (0x00469900). Neither has a source line to claim.
	// The virtual functions are named as the vftable 0x0079ebec names them (slot N is
	// vfunctionN+1). The vftable overrides slots 1, 2, 3, 12 and 14 of tGameState's.
	virtual bool vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
	virtual char *vfunction13();
	virtual void vfunction15(c_0046aa10_message *msg);

	void fn_004682c7();
	bool fn_00468527();
	void fn_00469271();
	void fn_00469534();
	void fn_00468297();
	bool fn_004683ff();
	bool fn_00468481();
	bool fn_0046858a();
	void fn_0046860d();
	// HYPOTHESIS: copies the text after the separator g_00844ec4, or all of it; types are guesses.
	void fn_00468677(c_00468677_source *source, tString &out);
	void fn_004686f6();
	bool fn_004688a9();
	// HYPOTHESIS: a member taking the address field and its name; the types are guesses.
	void fn_00468a45(c_00468677_source *control, const char *name);
	// HYPOTHESIS: fills the address field from the settings section name, or with value when
	// the section holds no entries.
	void fn_004688ee(c_00468677_source *control, const char *name, const char *value);
	// HYPOTHESIS: a member; it hides the four connection panels.
	void fn_00468bbb();
	// HYPOTHESIS: a member; it disables three controls.
	void fn_00468beb();
	// HYPOTHESIS: a member; it enables the controls fn_00468beb disables, the third only for
	// the control id 0x1001c.
	void fn_00468c13(int id);
	// HYPOTHESIS: a member; it polls the modem connection and shows its progress.
	void fn_00468c49();
	// HYPOTHESIS: a member; it stores the address field, then runs gNetControl.fn_00431821.
	void fn_00469096();
	bool fn_004690a5(int id);

	void fn_004695c4(const char *name, int n);

	void fn_00469636();

	unsigned char m0c;
	unsigned char m0d;
	unsigned char m0e;
	unsigned char m0f;
	unsigned char m10;
	unsigned char m11;
	char m12[2];
	int m14;
	unsigned char m18;
	unsigned char m19;
	unsigned char m1a;
	unsigned char m1b;
	tValueI< tString > m1c;
	tValueI< tString > m5c;
	// HYPOTHESIS: four more value objects, as the strings at +0x134 and +0x174 suggest. The
	// constructor (0x0046981c) builds 0x1c to 0x11c as tValueI< tString > (0x0043d2ef) but
	// 0x15c and 0x19c with another class (0x00436633, a vftable of 0x0079b8a4 stored after
	// 0x006230d0 at +0x18), so those two are not tValueI< tString > in the original.
	tValueI< tString > m9c;
	tValueI< tString > mdc;
	tValueI< tString > m11c;
	tValueI< tString > m15c;
	tValueI< tString > m19c;
	c_00468677_source *m1dc;
	c_00468677_source *m1e0;
	int m1e4;
	char *m1e8;
	// HYPOTHESIS: names the state hands to fn_004d32c4 on exit; the element type is a guess.
	std::vector< char * > m1ec;
	tString m1fc;
	int m224[6];
	int m23c;
};

typedef char tState_GameConnections_size_is_0x240[ sizeof( tState_GameConnections ) == 0x240 ? 1 : -1 ];

}	// namespace tState_Namespace_GameConnections

#endif	// tState_GameConnections_h
