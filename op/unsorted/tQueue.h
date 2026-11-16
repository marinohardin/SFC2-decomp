//	tQueue.h: the Tiger queue template, named by the exe's vtables (tQueue<tVolume>,
//	tQueue<twIORequest> and others).

#ifndef tQueue_h
#define tQueue_h

#pragma once

// HYPOTHESIS: a queue of 12 bytes, a vtable pointer and two words cleared by the constructor, as
// tFSSession::tFSSession stores them; the class is named by the exe, its members are not.
// HYPOTHESIS: the queue links nodes of 12 bytes, a vtable pointer, the queued item's address and
// the next node, as tQueue::pop (STARFLEETOP 0x006202c0) and tFSSession::~tFSSession read them;
// the node class and its members are not named by the exe.
template <class tType>
class tQueue
{
public:
	class fn_QueueNode
	{
	public:
		// HYPOTHESIS: the node copies the item, as twIORequestQueue::mAdd (STARFLEETOP 0x00634810)
		// allocates the copy after storing the node's vtable.
		fn_QueueNode( tType &item ) { fItem = new tType( item ); fNext = 0; }
		virtual ~fn_QueueNode();

		tType			*fItem;		// +0x04
		fn_QueueNode	*fNext;		// +0x08
	};

	tQueue() { f_04 = 0; f_08 = 0; }
	virtual ~tQueue();

	// HYPOTHESIS: out of line in the exe, as tSession::mAddCookie calls it.
	void push( tType &item );

	void pop()
	{
		fn_QueueNode	*node = f_04;

		f_04 = node->fNext;
		if ( f_04 == 0 )
			f_08 = 0;
		delete node;
	}

	fn_QueueNode	*f_04;	// head
	fn_QueueNode	*f_08;	// tail
};

#endif
