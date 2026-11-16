//	twIORequest.h: an asynchronous read or write on a path, and the manager of the queues that
//	carry them. The class, the constructor's argument types, twIORequestType and
//	twIORequestQueueManager::mRequest are named by the exe's symbols.

#ifndef twIORequest_h
#define twIORequest_h

#pragma once

#include "Tiger/tgfsys.h"

// HYPOTHESIS: the enumerators are not recovered; twPath::mRead (STARFLEETOP 0x006339a0) passes 0
// and twPath::mWrite (STARFLEETOP 0x00633bf0) passes 1.
enum twIORequestType
{
	fn_twIORequestType_0,
	fn_twIORequestType_1
};

// HYPOTHESIS: 0x24 bytes, as twPath::mRead copies them into twIORequestQueueManager::mRequest's
// argument: the type, the path, the buffer, the offset, the size and the callback.
class twIORequest
{
public:
	twIORequest( twIORequestType type, tPath path, void *buffer, unsigned long offset, unsigned long size, tPathCallback *callback );
	// HYPOTHESIS: the copy assigns in its body, as twPath::mRead's argument copy default constructs
	// the path before it copies the members.
	twIORequest( const twIORequest &other ) { *this = other; }
	~twIORequest();

	void	mProcessRequest( void );

	twIORequestType	f_00;
	tPath	f_04;
	void	*f_14;
	unsigned long	f_18;
	unsigned long	f_1c;
	tPathCallback	*f_20;
};

// HYPOTHESIS: a queue entry; AsynchIOMain (STARFLEETOP 0x00633fe0) processes the request at +4,
// follows the next entry at +8 and deletes the entry through its first virtual.
class fn_00633fe0_Node
{
public:
	virtual ~fn_00633fe0_Node();

	twIORequest	*f_04;
	fn_00633fe0_Node	*f_08;
};

// HYPOTHESIS: the layout is read from AsynchIOMain: the event the thread waits on at +4, the
// first and last entries at +0xc and +0x10, the state at +0x14 (-1 ends the thread) and the
// mutex at +0x18. The mutex handle is volatile, as AsynchIOMain loads it only after pushing the
// timeout at every wait and never keeps it in a register.
class twIORequestQueue
{
public:
	~twIORequestQueue();

	// HYPOTHESIS: inline, as AsynchIOMain tests the first entry under the mutex into a byte twice.
	bool	fn_00633fe0_IsEmpty( void );

	long	f_00;
	void	*f_04;
	long	f_08;
	fn_00633fe0_Node	*f_0c;
	fn_00633fe0_Node	*f_10;
	long	f_14;
	void	*volatile f_18;
};

inline bool twIORequestQueue::fn_00633fe0_IsEmpty( void )
{
	WaitForSingleObject( f_18, INFINITE );
	bool	empty = f_0c == 0;
	ReleaseMutex( f_18 );
	return empty;
}

class twIORequestQueueManager
{
public:
	void	mRequest( twIORequest request );
};

#endif
