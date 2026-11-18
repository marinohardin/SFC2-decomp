//	tState_NetSetup.h: tState_Namespace_NetSetup::tState_NetSetup, the network setup state.
// Evidence:  class (RTTI; original file unknown)

#ifndef tState_NetSetup_h
#define tState_NetSetup_h 1

#include <vector>
#include "Tiger/tstring.h"
#include "unsorted/tValueI.h"
#include "unsorted/tGameState.h"
#include "unsorted/c_0046aa10_msg.h"
#include "unsorted/c_00468677_source.h"

namespace tState_Namespace_NetSetup
{

// HYPOTHESIS: derives from tGameState like tState_GameConnections; only the slots rebuilt so
// far are declared, members placed by their offsets.
class tState_NetSetup : public tGameState
{
public:
	tState_NetSetup();
	virtual ~tState_NetSetup();
	virtual void vfunction1();
	virtual bool vfunction2();
	virtual void vfunction3();
	virtual bool vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	// HYPOTHESIS: returns the state's name.
	virtual char *vfunction13();
	virtual void vfunction14();
	virtual void vfunction15(c_0046aa10_message *msg);

	void fn_004764bd();
	void fn_004764ed();
	bool fn_00476678();
	bool fn_004766f1();
	bool fn_00476754();
	void fn_004767f1();
	bool fn_0047685b();
	void fn_004768a5(c_00468677_source *source, tString &out);
	bool fn_00476925();
	// HYPOTHESIS: puts the value into the control under the given key, as
	// tState_GameConnections::fn_004688ee does.
	void fn_00476997(c_00468677_source *control, const char *name, const char *value);
	// HYPOTHESIS: stores an address setting under the given key; callee pops its arguments.
	void fn_00476aee(c_00468677_source *control, const char *key);
	void fn_00476c64();
	void fn_00476c89();
	void fn_00476ce5(int id);
	// HYPOTHESIS: fills the control kept at the first pointer from the second; callee pops.
	void fn_00476d52(c_00468677_source **control, void *source, int id);
	void fn_00476da0(c_00468677_source **control, int id);
	void fn_00476e72();
	void fn_004772bf();
	bool fn_004772ce(int id);
	void fn_004776d7(const char *name, int n);
	void fn_00477749();

	void fn_00477440();
	void fn_00477689();

	unsigned char m0c;
	unsigned char m0d;
	unsigned char m0e;
	unsigned char m0f;
	unsigned char m10;
	unsigned char m11;
	char m12[2];
	int m14;
	unsigned char m18;
	char m19[3];
	tValueI< tString > m1c;
	tValueI< tString > m5c;
	tValueI< tString > m9c;
	tValueI< tString > mdc;
	tValueI< tString > m11c;
	tValueI< tString > m15c;
	tValueI< tString > m19c;
	c_00468677_source *m1dc;
	// HYPOTHESIS: m1e0, m1e8 and m1ec are controls of the same kind as m1dc (fn_00476c89 calls
	// slot 65 on them).
	c_00468677_source *m1e0;
	c_00468677_source *m1e4;
	c_00468677_source *m1e8;
	c_00468677_source *m1ec;
	int m1f0;
	char *m1f4;
	// HYPOTHESIS: names the state hands to fn_004d32c4 on exit, as in tState_GameConnections.
	std::vector< char * > m1f8;
	tString m208;
};

}	// namespace tState_Namespace_NetSetup

#endif	// tState_NetSetup_h
