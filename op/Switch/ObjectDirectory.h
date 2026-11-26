//	Switch/ObjectDirectory.h
//
//	nSwitch::tObjectDirectory, the table of objects a switch keeps by address, and the base of
//	tCentralObjectDirectory and tProxyObjectDirectory (neither adds a member: both are 0x17c
//	bytes, as the allocations in the tSwitch constructors show).
//
//	HYPOTHESIS: the original's header is not recovered. The class and its bases are named by the
//	exe's RTTI (.?AVtObjectDirectory@nSwitch@@: tDataHandler, then tRefCount); the layout by the
//	constructor (STARFLEETOP 0x00727a90) and the destructor (0x00727d20), the slots by the
//	vtable at 0x007bba68. No member or virtual name is recovered, so each is named by its offset
//	or its slot, and an argument or result whose type the exe does not show is a 32-bit word.

#if !defined( ObjectDirectory_h )
#define ObjectDirectory_h

#include "Common/BaseTypes.h"
#include "Common/SmartPtr.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/Thread.h"
#include "Switch/DataHandler.h"
#include "unsorted/nSwitch__tAddress.h"

namespace nSwitch
{

	class tObjectDirectory : public tDataHandler, public tRefCount
	{
	public:
		// The constructor (STARFLEETOP 0x00727a90) takes the switch it belongs to, as the tSwitch
		// constructors pass themselves; it asks slot 14 for the address of that switch and of
		// itself, and keeps the second in +0xbc.
		tObjectDirectory( tDataHandler *owner );
		virtual ~tObjectDirectory();		// slot 10, STARFLEETOP 0x00727d00 (deleting)

		// Slots 0 to 10 are tDataHandler's; this class replaces slot 5 and the destructor.
		// Slot 5 (0x0072a2f0) is an asserting body in ObjectDirectory.cpp, line 0x27b.
		virtual void	fn_slot5( int32u arg1, int32u arg2 );

		// Slots 11 to 31 follow the exe's table at 0x007bba68 in order. The ones named below
		// are overridden by tCentralObjectDirectory or tProxyObjectDirectory. A result passed
		// through a hidden first argument is declared as that argument and returned.
		// Slot 11 (0x00728120): walks the table at +0xe4 and the list at +0xfc with its argument.
		virtual void	fn_slot11( int32u arg );
		// Slot 12 (0x00728430): sets the smart pointer at +0xe0; the argument is a smart
		// pointer passed by value and may not be null (assertion text "pCentralID.IsValid()").
		virtual void	fn_slot12( tSmartPtr<tAddress> arg );
		// Slot 13 (0x007284f0): clears the pointer at +0xe0, then calls slot 11 with -1.
		virtual void	fn_slot13( void );
		// Slot 14 (0x007288b0): allocates a 0x48-byte address object for the handler passed.
		virtual tSmartPtr<tAddress>	fn_slot14( tDataHandler *handler );
		// Slot 15 (0x00728730).
		virtual int32u*	fn_slot15( int32u *result, int32u arg1, int32u arg2, int32u arg3,
							int32u arg4 );
		// Slot 16 (0x00728a90).
		virtual void	fn_slot16( int32u arg1, int32u arg2 );
		// Slot 17 (0x00729480).
		virtual void	fn_slot17( int32u arg );
		// Slot 18 (0x00729260).
		virtual void	fn_slot18( int32u arg1, int32u arg2, int32u arg3 );
		// Slots 19 and 20 (0x00729690, 0x007296a0) set and clear the flag at +0x144; the
		// second one calls slot 31 before and after.
		virtual void	fn_slot19( void );
		virtual void	fn_slot20( void );
		// Slot 21 is the exe's pure-virtual stub in this class's table (0x00737f86);
		// tCentralObjectDirectory (0x00722940) and tProxyObjectDirectory (0x00724250) fill it.
		virtual int32u*	fn_slot21( int32u *result, int32u arg1, int32u arg2 ) = 0;
		// Slot 22 (0x007299b0) and slot 23 (0x00729cc0, which takes a pointer).
		virtual void	fn_slot22( int32u arg );
		virtual void	fn_slot23( int32u *arg );
		// Slot 24 (0x0072a2c0): returns the smart pointer at +0xe0.
		virtual tSmartPtr<tAddress>	fn_slot24( void );
		// Slot 25 (0x00728520): finds a free place in the vector at +0x114, or appends, stores
		// the argument there and returns its index (assertion text "newObjectID >
		// tAddress::kInvalid").
		virtual int32u	fn_slot25( int32u arg );
		// Slot 26 (0x00728b90).
		virtual int32u*	fn_slot26( int32u *result, int32u arg1, int32u arg2 );
		// Slot 27 (0x00729c00): its argument is a byte.
		virtual void	fn_slot27( int32u arg );
		// Slot 28 (0x00728f60), slot 29 (0x00729840), slot 30 (0x0072a110).
		virtual void	fn_slot28( int32u arg1, int32u arg2 );
		virtual int32u	fn_slot29( int32u arg );
		virtual int32u	fn_slot30( int32u arg );
		// Slot 31 (0x007296c0).
		virtual void	fn_slot31( void );

	protected:
		// +0xbc: the address object slot 14 made for this directory, assigned in the
		// constructor.
		tSmartPtr<tAddress>			f_bc;
		// +0xc0: an interlocked value, built with -1.
		tInterlockedScalar<long>	f_c0;
		// +0xe0: the central directory's address; tCentralObjectDirectory copies f_bc into it.
		tSmartPtr<tAddress>			f_e0;

		// The five members below are each a tSingleAccessWrapper: a pointer, then the tOptex
		// that guards it (0x18 bytes). Each pointer is allocated by the constructor except
		// the one at +0x12c; the destructor deletes them in the reverse order.
		// +0xe4: a std::map whose nodes are 0x44 bytes, the last pointer in each node at +0x3c
		// holding an object the destructor deletes.
		void		*f_e4;
		tOptex		f_e8;
		// +0xfc: a std::vector (0x10 bytes) of 4-byte words.
		void		*f_fc;
		tOptex		f_100;
		// +0x114: a std::vector of 4-byte words; slot 25 keeps the addresses it hands out here.
		void		*f_114;
		tOptex		f_118;
		// +0x12c: a std::vector of 0x30-byte objects with a virtual destructor, like the member
		// at tDataHandler +0x7c. The constructor clears it.
		void		*f_12c;
		tOptex		f_130;
		// +0x144: an interlocked flag, built false; slots 19 and 20 set and clear it.
		tInterlockedScalar<bool>	f_144;
		// +0x164: a std::vector (0x10 bytes) of 4-byte words.
		void		*f_164;
		tOptex		f_168;
	};

	typedef char tObjectDirectory_size_is_0x17c[sizeof(tObjectDirectory) == 0x17c ? 1 : -1];

};

#endif
