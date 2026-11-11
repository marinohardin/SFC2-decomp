//	tShipCommand.h: tShipCommand, an order given to a ship, and the commands derived from it.
// Evidence:  class (RTTI; original file unknown)

#ifndef tShipCommand_h
#define tShipCommand_h 1

// HYPOTHESIS: declared only as far as tAICaptain uses it. 0x0051e33f takes one and appends what
// its slot 1 returns to the captain's list; the captain destroys a command on its stack through
// the destructor at 0x005241ab whichever command it built, so the derived commands declare none.
// Size 0x14, from the stack slots the captain gives each command.
class tShipCommand
{
public:
	virtual ~tShipCommand();
	virtual int vfunction1();

	char field_0x4[0x10];                   // +0x4
};

// HYPOTHESIS: constructor 0x005243bf; tAICaptain passes 1 and 10, meanings unknown.
class tCommandDisengage : public tShipCommand
{
public:
	tCommandDisengage(int param_1, int param_2);
};

// HYPOTHESIS: constructor 0x00524519; tAICaptain passes 0, meaning unknown.
class tCommandClearCurrentOrders : public tShipCommand
{
public:
	tCommandClearCurrentOrders(int param_1);
};

#endif	// tShipCommand_h
