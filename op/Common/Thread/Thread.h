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
	tThread();
	~tThread();

	bool StartWork( tCallback<int32>* workItem, int32 context );
	void StopThread( int32u timeout );

	bool Working() const;
	bool Alive() const;

protected:
	// HYPOTHESIS: the fields are not declared here; only the size (0xd0c, the allocation in
	// QueueWorkItem) is known.
	char	fUnknown[0xd0c];
};

#endif
