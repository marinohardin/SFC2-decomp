//	tNetControl.h: tNetControl, the network control, and its object at 0x008f5b98.
// Evidence:  class (original file unknown)

#ifndef tNetControl_h
#define tNetControl_h 1

#include <vector>

#include "Tiger/tstring.h"

class c_tNetControl_8D00;
class c_0043127f_node;
struct tNetMsg;
struct c_tNetControl_8CC4_entry;	// HYPOTHESIS: defined in tNetControl.cpp, its only user
class tNetControl
{
public:
	tNetControl();
	virtual ~tNetControl();
	// HYPOTHESIS: sets field_10 to field_11 plus an offset, kept within 1..6 (6 otherwise).
	void fn_00431159( unsigned char n );
	void fn_00431200();
	void fn_00431227();
	void fn_0043124e();
	void fn_0043127f();
	// HYPOTHESIS: finds the node in the field_8CB8 list with the given id, or 0.
	c_0043127f_node *fn_004314f5( int id );
	// HYPOTHESIS: finds the node in the field_8CB8 list whose field_70 equals the argument, or 0.
	c_0043127f_node *fn_00431523( int id );
	c_0043127f_node *fn_00431551();
	void fn_00431571();
	void fn_004315ee();
	void fn_004316cf();
	void fn_00431708();
	void fn_00431ee0();
	int fn_004317bd();
	void fn_004317c3();
	void fn_004317ce( int a, int b );
	void fn_004317eb( int a, int b );
	void fn_00431808( int a );
	void fn_004321fd( int n );
	bool fn_00432597();	// HYPOTHESIS: returns a flag (tested in al)
	int fn_004326a4();
	void fn_00431821();
	bool fn_0043268d();	// HYPOTHESIS: returns a flag (tested in al)
	void fn_00432f5a();
	// HYPOTHESIS: (message, command, destination, time); 0x00413541 passes 0.0 as the time.
	void mQueueMsg(tNetMsg *aMsg, int aCommand, int aDestination, double aTime);
	// HYPOTHESIS: (message, command, player, destination, time); sends to one player's entry
	// at 0x008f5b48 instead of every player.
	void mQueueMsg(tNetMsg *aMsg, int aCommand, int aPlayer, int aDestination, double aTime);

	// HYPOTHESIS: field types are from the constructor's stores only; names are unknown.
	// The double at +0x8d38 aligns the class to 8, so the vfptr slot is padded to +0x8.
	unsigned char field_8;
	float field_C;
	unsigned char field_10;
	unsigned char field_11;
	unsigned char field_12[ 0x8cb4 - 0x12 ];
	int field_8CB4;
	int field_8CB8;
	int field_8CBC;
	int field_8CC0;
	std::vector< c_tNetControl_8CC4_entry > field_8CC4;
	int field_8CD4;
	int field_8CD8;
	int field_8CDC;
	int field_8CE0;
	int field_8CE4;
	int field_8CE8;
	float field_8CEC;
	std::vector< int > field_8CF0;	// HYPOTHESIS: element type unknown (no destructor)
	c_tNetControl_8D00 *field_8D00;
	unsigned char field_8D04;
	unsigned char field_8D05;
	unsigned char field_8D06;
	tString field_8D08;
	unsigned char field_8D30;
	double field_8D38;
};

// The network control object at 0x008f5b98.
extern tNetControl gNetControl;

#endif	// tNetControl_h
