//	twIORequest.h: an asynchronous read or write on a path, and the manager of the queues that
//	carry them. The class, the constructor's argument types, twIORequestType and
//	twIORequestQueueManager::mRequest are named by the exe's symbols.

#ifndef twIORequest_h
#define twIORequest_h

#pragma once

#include "Tiger/tgfsys.h"

// HYPOTHESIS: the enumerators are not recovered; twPath::mRead (STARFLEETOP 0x006339a0) passes 0.
enum twIORequestType
{
	fn_twIORequestType_0
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

// HYPOTHESIS: inline here, as twPath::mRead leaves no call to it; the out-of-line copy is
// STARFLEETOP 0x00633be0.
inline twIORequest::~twIORequest()
{
}

class twIORequestQueueManager
{
public:
	void	mRequest( twIORequest request );
};

#endif
