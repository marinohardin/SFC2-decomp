// Common/Thread/Thread.h

#if !defined( Thread_h )
#define Thread_h

#include "Common/BaseTypes.h"
#include "Common/Callback.h"
#include "Common/Thread/Optex.h"
#include "Common/Thread/ThreadEvent.h"
#include "Common/Thread/ResourceGuard.h"
#include "Common/DataStore/Streamable.h"

// HYPOTHESIS: the SDK declares tInterlockedType, tInterlockedScalar and tCallStack in
// Common/Thread/InterlockedType.h, Common/Thread/InterlockedScalar.h and
// Common/Debug/CallStack.h; they stay here, in the SDK's form and with only what tThread
// uses, until those headers exist.

// CLASS PURPOSE: Instances of this class will be accessed by multiple
// threads. So, all members of this class (except the constructor and
// destructor) must be thread-safe.
template< typename tType >
class tInterlockedType
{
public:
//Setup
	// Note: Constructors & destructors are always thread safe
	tInterlockedType( const tType& value );
	virtual ~tInterlockedType();

//Accessors
	operator tType() const;

protected:
//Operations
	virtual void OnValChanged( const tType& newValue, const tType& prevValue ) const;

//Fields
	// Guard for use by derived class functions
	mutable tResourceGuard fResourceGuard;

private:
//Hidden
	tInterlockedType( const tInterlockedType& copy );
	tInterlockedType& operator=( const tInterlockedType& copy );

//Fields
protected:
	tType fValue;
};

template< typename tType >
inline tInterlockedType<tType>::tInterlockedType( const tType& value )
{
	fValue = value;
}

template< typename tType >
inline tInterlockedType<tType>::operator tType() const
{
	tResourceGuard::tGuard guard( fResourceGuard );
	return fValue;
}

template< typename tType >
inline tInterlockedType<tType>::~tInterlockedType()
{
}

template< typename tType >
inline void tInterlockedType< tType >::OnValChanged( const tType& newValue, const tType& prevValue ) const
{
}

// CLASS PURPOSE:
// Instances of this class will be accessed by multiple threads. So,
// all members of this class (except the constructor and destructor)
// must be thread-safe.
template< typename tType >
class tInterlockedScalar : public tInterlockedType< tType >
{
public:
//Setup
	tInterlockedScalar( tType value = tType() );
	~tInterlockedScalar();

//Operations
	tInterlockedScalar& operator=( const tType& value );
};

template< typename tType >
inline tInterlockedScalar<tType>& tInterlockedScalar<tType>::operator=( const tType& value )
{
	tResourceGuard::tGuard guard( fResourceGuard );
	tType prev = fValue;
	if( prev != value )
	{
		fValue = value;
		OnValChanged( value, prev );
	}
	return *this;
}

template< typename tType >
inline tInterlockedScalar<tType>::tInterlockedScalar( tType value ) :
	tInterlockedType<tType>( value )
{
}

template< typename tType >
inline tInterlockedScalar<tType>::~tInterlockedScalar()
{
}

class tCallStack : public nDataStore::tStreamable
{
public:
// Setup
	tCallStack( bool recordCurStack = true );
	virtual ~tCallStack();

// Operations
	virtual	void StreamOut( nDataStore::tOutput& output ) const;
	virtual	void StreamIn( nDataStore::tInput& input );
	void ClearStackInfo();

protected:
// Types
	enum
	{
		kMaxStackTrace = 200
	};

// Fields
	int32u			fCallStackSize;
	std::string		fCallStack[ kMaxStackTrace ];
};

// CLASS PURPOSE: A class that encapsulates threading
class tThread
{
public:
// Types
	typedef tCallback< int32 > iThreadCallback;

// Setup
	tThread();
	~tThread();

// Operations
	bool StartWork( iThreadCallback* workItem, int32 context );
	void StopThread( int32u waitTime = 0 );

	// HYPOTHESIS: the enumerators are the SDK's; the original's names and values are not
	// recovered (the symbol of 0x006ff6d0 only names the type).
	enum ThreadPriority
	{
		kThreadPriorityIdle = 0,
		kThreadPriorityLow,
		kThreadPriorityBelowNormal,
		kThreadPriorityNormal,
		kThreadPriorityAboveNormal,
		kThreadPriorityHigh,
		kThreadPriorityRealTime
	};

	void mSetPriority( ThreadPriority priority );

// Accessors
	bool Working() const;
	bool Alive() const;

protected:
// Operations
	// HYPOTHESIS: the thread's work loop (the SDK's Work), 0x006ff710; no symbol names it.
	void fn_006ff710();
	static unsigned int __stdcall ThreadProc( void* param );

// Fields
	// HYPOTHESIS: fThread is the thread handle; the SDK also has fWaitingForThreadID and
	// fWaitingForOptex here, this build's layout has no room for them.
	int32u			fThread;			// 0x00
	tThreadEvent	fWorktimeEvent;		// 0x04
	tThreadEvent	fShutdownCompleteEvent;	// 0x10
	unsigned int	fThreadID;			// 0x1c

	// HYPOTHESIS: Working() reads fWorking, Alive() fAlive.
	tInterlockedScalar< bool >	fWorking;	// 0x20
	tInterlockedScalar< bool >	fAlive;		// 0x40
	iThreadCallback* fWorkItem;			// 0x60
	int32			fContext;			// 0x64
	tOptex			fStartUpGuard;		// 0x68, built with (kUseThreadObject, 4000)

	tCallStack		fCallStackOfCallingThread;	// 0x7c

private:
//Hidden
	tThread( const tThread& copy );
	tThread& operator=( const tThread& copy );
};

typedef char tThread_size_check[ sizeof( tThread ) == 0xd0c ? 1 : -1 ];

#endif
