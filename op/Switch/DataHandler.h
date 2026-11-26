//	Switch/DataHandler.h
//
//	nSwitch::tDataHandler, the base of tSwitch, tAsyncDataHandler, tMessageCache and the object
//	directories. The units are Switch/DataHandler.cpp (assertion path) and the class's others
//	in unsorted/.
//
//	HYPOTHESIS: the original's header is not recovered. The class is named by the exe's RTTI
//	(.?AVtDataHandler@nSwitch@@), its layout by the constructor (STARFLEETOP 0x0071c7b0) and the
//	slots below by the vtable at 0x007bb7b8; no member or virtual name is recovered, so each is
//	named by its offset or its slot.

#if !defined( DataHandler_h )
#define DataHandler_h

#include "Common/BaseTypes.h"
#include "Common/SmartPtr.h"
#include "Common/MemberCallback.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/Thread.h"
#include "unsorted/nSwitch__tAddress.h"

// 0xb4 bytes, as the constructor fills them and tObjectDirectory puts its tRefCount base at +0xb4.
namespace nSwitch { class tDataHandler
	{
	public:
		// The slots are the exe's vtable at 0x007bb7b8 in order; the slot numbers count from 0.
		// HYPOTHESIS: slot 0 (STARFLEETOP 0x0071ca10) stores its argument at +0xb0 under the
		// guard at +0x04.
		virtual void	fn_slot0( int32u arg );
		// Slots 1 and 2 (0x0071cc20, 0x0071cdf0) take no argument; both lock the guard at +0x04.
		// HYPOTHESIS: slot 1 returns a bool; slot 6 tests it (0x0071d010).
		virtual bool	fn_slot1( void );
		virtual void	fn_slot2( void );
		// Slot 3 (0x00637810) returns true; slot 4 (0x006c9980) does nothing. Both are shared
		// with other classes' vtables.
		virtual bool	fn_slot3( void );
		virtual void	fn_slot4( void );
		// Slot 5 is the exe's pure-virtual stub in this class's own table (0x00737f86);
		// tObjectDirectory fills it with an asserting body that takes two arguments (0x0072a2f0).
		virtual void	fn_slot5( int32u arg1, int32u arg2 ) = 0;
		// Slot 6 (0x0071d010), 4 bytes of arguments: calls slot 1 of the table (+0x04), then
		// hands the argument and the callback at +0x78 on.
		virtual void	fn_slot6( int32u arg );
		// Slot 7 (0x0071d1a0): registers the member at +0x7c with the switch and returns the
		// smart pointer at +0xac through a hidden return slot, like slot 9.
		virtual tSmartPtr<tAddress>	fn_slot7( void );
		// Slot 8 (0x0071d1e0): returns the address of the member at +0x7c.
		virtual int32u*	fn_slot8( void );
		// Slot 9 (0x0071d1f0): returns the smart pointer at +0xac.
		virtual tSmartPtr<tAddress>	fn_slot9( void );

		virtual ~tDataHandler();	// slot 10, STARFLEETOP 0x0071c8a0

		tDataHandler();

		// Not in the vtable (0x0071d090): forwards its argument to the member at +0x7c.
		void	fn_0071d090( int32u arg );

		// Not in the vtable (0x0071d0a0): sets the flag at +0x58 and the smart pointer at +0xac.
		void	fn_0071d0a0( tSmartPtr<tAddress> address );

		// Not in the vtable (0x0071d170): calls 0x005d8e25, then drops the smart pointer at +0xac.
		void	fn_0071d170( void );

		// Not in the vtable (0x0071d220): the work item the callback at +0x78 runs for slot 6.
		void	fn_0071d220( tRefCount* packet );

	protected:
		// +0x04: the spin-counted guard slots 0 to 2 lock, built as tOptex( use, 4000 ).
		tOptex						f_04;
		// +0x18 (value 0), +0x38 (value 0) and +0x58 (value 1): interlocked values, whose
		// types the vtables at 0x007bb7e8 and 0x007b4adc name.
		tInterlockedScalar<long>	f_18;
		tInterlockedScalar<bool>	f_38;
		tInterlockedScalar<bool>	f_58;
		// +0x78: a tMemberCallback<tDataHandler,long>, allocated 0xc bytes at the constructor
		// with the handler and the member at 0x0071d220.
		tMemberCallback<tDataHandler,long>	*f_78;
		// +0x7c: 0x30 bytes built by the routine at 0x00714da0; slot 8 returns their address.
		// The words are not recovered; the object starts with a tStreamable.
		int32u						f_7c[12];
		// +0xac: the smart pointer slots 7 and 9 return.
		tSmartPtr<tAddress>			f_ac;
		// +0xb0: the argument slot 0 stores; the constructor clears it.
		// HYPOTHESIS: the destructor sets the byte it points to (0x0071c8c0), so it is a pointer.
		bool						*f_b0;
	};

	typedef char tDataHandler_size_is_0xb4[sizeof(tDataHandler) == 0xb4 ? 1 : -1];

};

#endif
