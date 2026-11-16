//	twIORequest.h: an asynchronous read or write on a path, and the manager of the queues that
//	carry them. The class, the constructor's argument types, twIORequestType and
//	twIORequestQueueManager::mRequest are named by the exe's symbols.

#ifndef twIORequest_h
#define twIORequest_h

#pragma once

#include "Tiger/tgfsys.h"
#include "Tiger/tstring.h"
#include "unsorted/tQueue.h"

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

// HYPOTHESIS: a string naming a request queue; the exe names the class and its copy constructor
// (STARFLEETOP 0x00635310), and twIORequestQueue::twIORequestQueue copies it into its buffers
// and destroys it as a tString.
class twQueueName: public tString
{
public:
	twQueueName( const twQueueName &other );
};

// HYPOTHESIS: the layout is read from AsynchIOMain and the constructor: the thread at +0, the
// semaphore the thread waits on at +4, the queue of requests at +8 (the exe's tQueue<twIORequest>
// vtable), the state at +0x14 (-1 ends the thread) and the mutex at +0x18. The mutex handle is
// volatile, as AsynchIOMain loads it only after pushing the timeout at every wait and never keeps
// it in a register.
class twIORequestQueue
{
public:
	twIORequestQueue( twQueueName name );
	~twIORequestQueue();

	// HYPOTHESIS: inline, as AsynchIOMain tests the first entry under the mutex into a byte twice.
	bool	fn_00633fe0_IsEmpty( void );

	void	*f_00;
	void	*f_04;
	tQueue<twIORequest>	f_08;
	long	f_14;
	void	*volatile f_18;
};

inline bool twIORequestQueue::fn_00633fe0_IsEmpty( void )
{
	WaitForSingleObject( f_18, INFINITE );
	bool	empty = f_08.f_04 == 0;
	ReleaseMutex( f_18 );
	return empty;
}

class twIORequestQueueManager
{
public:
	void	mRequest( twIORequest request );
};

#endif
