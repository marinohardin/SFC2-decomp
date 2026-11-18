//	fn_004D3143.h: the cdecl helpers of fn_004D3143.cpp that other units call.
// Evidence:  calls (original names and file unknown)

#ifndef fn_004D3143_h
#define fn_004D3143_h 1

class tString;
class tValueBase;
class c_00468677_source;

// HYPOTHESIS: the parameter types are the ones the callers in tState_NetSetup and
// tState_GameConnections pass (control names, a value object, a control id); the widths of the
// bool parameters are the byte compares in the bodies (0x004d34c5, 0x004d35bf), and
// 0x004d31ed returns the found flag in al.
void fn_004d3179( const char* name, const tString& value );
bool fn_004d31ed( const char* name, int arg, void* p, int width, int height );
void fn_004d32c4( const char* name );
void fn_004d3331( const char* name );
void fn_004d3340( const char* name );
// HYPOTHESIS: returns the control with the given id.
c_00468677_source* fn_004d3446( int id );
// HYPOTHESIS: the second argument is the group's name.
void fn_004d346f( const char* name, const char* group );
// HYPOTHESIS: enables (true) or disables (false) the control with the given id.
void fn_004d34c5( int id, bool enable );
void fn_004d3564( const char* psz1, const char* psz2, int n );
// HYPOTHESIS: binds a value object to a control name; registers a control name; unregisters
// one; and unbinds a value object from a name, returning it (deleted first when asked).
void fn_004d35ac( tValueBase* value, const char* name );
tValueBase* fn_004d35bf( const char* name, bool del );
void fn_004d35e3( const char* name, const char* group, int n );
void fn_004d35fa( const char* name );
void fn_004d3774( int id );

#endif	// fn_004D3143_h
