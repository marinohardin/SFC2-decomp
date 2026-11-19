// Header of: Common/Thread/Thread.cpp
#ifndef COMMON_THREAD_THREAD_H
#define COMMON_THREAD_THREAD_H

typedef long			int32;
typedef unsigned long	int32u;

template< typename tnResult >
class tCallback;

class tThread
{
public:
	// HYPOTHESIS: the enumerators are not known; the name comes from the SDK library symbol
	// matched to 0x006ff6d0.
	enum ThreadPriority
	{
	};

	tThread();
	~tThread();

	bool StartWork( tCallback<int32>* workItem, int32 context );
	void mSetPriority( ThreadPriority priority );
	void StopThread( int32u timeout );

	bool Working() const;
	bool Alive() const;

protected:
	// HYPOTHESIS: the fields are not declared here; only the size (0xd0c, the allocation in
	// QueueWorkItem) is known.
	char	fUnknown[0xd0c];
};

#endif
