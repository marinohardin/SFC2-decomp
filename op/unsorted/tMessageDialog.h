//	tMessageDialog.h: tMessageDialog and tMessageDialogGlobal, the dialogs that show a message.
// Evidence:  class (RTTI: tMessageDialog vftable 0x007b5c74, tMessageDialogGlobal 0x007b5c88, whose
// bases are tMessageDialog and tTopmostWindow; original file unknown)

#ifndef tMessageDialog_h
#define tMessageDialog_h 1

#include <string>

// vftable 0x007b5c80, stored first by tMessageDialog's constructors; its one slot is 0x00737f86,
// which calls the pure-call handler.
class tTopmostWindow
{
public:
	virtual void vfunction1() = 0;
};

// Size 0x2c (0x00600ab8 allocates the global one with 0x2c bytes). Constructor 0x006005aa takes
// nothing; 0x00600620 takes two strings, copies them to the two members and shows the dialog
// (0x0060072f with 5 and 1).
class tMessageDialog : public tTopmostWindow
{
public:
	tMessageDialog();
	tMessageDialog( const std::string &aFirst, const std::string &aSecond );
	virtual void vfunction1();	// 0x00600a4c
	virtual ~tMessageDialog();	// 0x006006ef; the deleting form is 0x00600604

	void fn_0060072f( int param_1, char param_2 );
	void fn_006007f9( int param_1 );
	void fn_00600857( void* param_1 );
	void fn_00600922( void* param_1 );
	void fn_006009ed();

	std::string field_04;
	std::string field_14;
	int field_24;
	char field_28;
};
typedef char tMessageDialog_size_check[ sizeof( tMessageDialog ) == 0x2c ? 1 : -1 ];

// vftable 0x007b5c88: slot 0 is tMessageDialog's, slot 1 the deleting destructor 0x00600b2e
// (body 0x00600bda). It adds no members.
class tMessageDialogGlobal : public tMessageDialog
{
public:
	virtual ~tMessageDialogGlobal();
};
typedef char tMessageDialogGlobal_size_check[ sizeof( tMessageDialogGlobal ) == 0x2c ? 1 : -1 ];

#endif	// tMessageDialog_h
