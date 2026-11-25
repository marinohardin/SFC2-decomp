//	tShipCommand.h: tShipCommand, an order given to a ship, and the commands derived from it.
// Evidence:  class (RTTI; original file unknown)

#ifndef tShipCommand_h
#define tShipCommand_h 1

// Constructor 0x0052416e stores its two arguments at +0x4 and +0x8 and clears the bytes at +0xc
// and +0xd. The derived commands add their own data from +0x10 (0x14 bytes each, the size slot 1
// allocates and the captain gives each command on its stack). 0x0051e33f takes one and appends
// what its slot 1 returns to the captain's list; slot 1 of the derived commands clones the command
// (0x005243e0, 0x00524538 copy +0x4, +0x8 and their own data into a new 0x14-byte object). The
// captain destroys a command on its stack through the destructor at 0x005241ab whichever command
// it built, so the derived commands declare none.
class tShipCommand
{
public:
	tShipCommand(int param_1, int param_2);
	virtual ~tShipCommand();
	virtual int vfunction1();

	int field_4;                            // +0x4, the first constructor argument: the command's type (0xb disengage, 0x10 clear current orders)
	int field_8;                            // +0x8, the second constructor argument (10 for both commands here)
	char field_c;                           // +0xc, cleared by the constructor
	char field_d;                           // +0xd, cleared by the constructor
};

// Constructor 0x005243bf passes 0xb and its second argument to the base and stores its first at +0x10.
class tCommandDisengage : public tShipCommand
{
public:
	tCommandDisengage(int param_1, int param_2);
	virtual int vfunction1();               // 0x005243e0, clones the command

	int field_10;                           // +0x10, the first constructor argument; tAICaptain passes 1 (and 10 as the second)
};

// Constructor 0x00524519 passes 0x10 and 10 to the base; its own argument (a byte, 0 from tAICaptain)
// is not stored. The clone made by 0x00524538 copies the byte at +0x10.
class tCommandClearCurrentOrders : public tShipCommand
{
public:
	tCommandClearCurrentOrders(int param_1);
	virtual int vfunction1();               // 0x00524538, clones the command

	char field_10;                          // +0x10, copied by the clone; the constructor leaves it alone
};

// Last in the file: a typedef between declarations once changed another function's score.
typedef char tShipCommand_size_is_0x10[ sizeof( tShipCommand ) == 0x10 ? 1 : -1 ];
typedef char tCommandDisengage_size_is_0x14[ sizeof( tCommandDisengage ) == 0x14 ? 1 : -1 ];
typedef char tCommandClearCurrentOrders_size_is_0x14[ sizeof( tCommandClearCurrentOrders ) == 0x14 ? 1 : -1 ];

#endif	// tShipCommand_h
