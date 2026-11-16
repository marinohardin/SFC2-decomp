//	twIORequest.h: an asynchronous read or write on a path, and the manager of the queues that
//	carry them. The class, the constructor's argument types, twIORequestType and
//	twIORequestQueueManager::mRequest are named by the exe's symbols.

#ifndef twIORequest_h
#define twIORequest_h

#pragma once

#include "Tiger/tgfsys.h"
#include "Tiger/tstring.h"
#include "unsorted/tQueue.h"
#include <map>

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
// and destroys it as a tString. It holds the string rather than deriving from it: the exe has no
// vtable or type descriptor of its own for it, and twIORequestQueueManager's destructor constructs
// and destroys one as a bare tString.
class twQueueName
{
public:
	// HYPOTHESIS: an inline constructor from a string that assigns in its body, and a destructor
	// of its own: twIORequestQueueManager's destructor builds a name this way and its unwind table
	// tracks the string and the whole name as two states.
	twQueueName( const tString &name ) { f_00 = name; }
	twQueueName( const twQueueName &other );
	~twQueueName() {}

	tString	f_00;
};

// HYPOTHESIS: the map of queues orders the names as strings.
inline bool operator<( const twQueueName &a, const twQueueName &b )
{
	return a.f_00 < b.f_00;
}

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

	void	mAdd( twIORequest request );

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
	~twIORequestQueueManager();

	void	mRequest( twIORequest request );

	// HYPOTHESIS: the names of the open queues at +0 and the queue of each name at +0xc, as the
	// destructor reads them; the exe names both instantiations, not the members.
	tQueue<twQueueName>	f_00;
	std::map<twQueueName, twIORequestQueue *>	f_0c;
};

#endif
