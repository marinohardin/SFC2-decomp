// Common/Thread/ThreadPool.h

#if !defined( ThreadPool_h )
#define ThreadPool_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <list>

#include "Common/BaseTypes.h"
#include "Common/Thread/Optex.h"
#include "Common/Timer/PerformanceCounter.h"

class tThread;

template< typename tnResult >
class tCallback;

// PORT: the game was built with the SDK's replacement STL headers, whose deque keeps a map
// pointer, its size, an offset and a count after the allocator; the compiler's own <deque> has a
// different layout. Only the members this unit reaches are declared; the out-of-line ones are
// the instantiations the game already holds. A unit that has included the compiler's own <deque>
// cannot use this header: unlike OSVersion.h's auto_ptr, the deque's layout is part of
// tThreadPool's, so taking the other one would silently move its fields.
#if defined( _DEQUE_ ) || defined( _LIBCPP_DEQUE ) || defined( _GLIBCXX_DEQUE )
#error ThreadPool.h declares the game's own std::deque; a unit that includes it must not include <deque>
#endif
namespace std
{
	template<class _Ty, class _A>
	class _Deque_val
	{
	protected:
		_Deque_val( _A _Al = _A() )
			: _Alval( _Al )
		{}

		_A _Alval;
	};

	template<class _Ty, class _A = allocator<_Ty> >
	class deque
		: public _Deque_val<_Ty, _A>
	{
	public:
		typedef _Deque_val<_Ty, _A> _Mybase;
		typedef unsigned int size_type;

		deque()
			: _Mybase(), _Map( 0 ), _Mapsize( 0 ), _Offset( 0 ), _Size( 0 )
		{}
		~deque()
		{
			clear();
		}

		bool empty() const
		{
			return ( _Size == 0 );
		}
		size_type size() const
		{
			return _Size;
		}
		_Ty& front()
		{
			size_type block = _Offset / 512;
			size_type index = _Offset - block * 512;
			if( _Mapsize <= block )
			{
				block -= _Mapsize;
			}
			return _Map[block][index];
		}
		void pop_front()
		{
			if( !empty() )
			{
				if( _Mapsize * 512 <= ++_Offset )
				{
					_Offset = 0;
				}
				if( --_Size == 0 )
				{
					_Offset = 0;
				}
			}
		}
		void push_back( const _Ty& _X );
		void pop_back();
		void clear()
		{
			while( !empty() )
			{
				pop_back();
			}
			_Freemap();
		}

	protected:
		void _Freemap();

		_Ty**		_Map;
		size_type	_Mapsize, _Offset, _Size;
	};
};

class tThreadPool
{
public:
	// HYPOTHESIS: the value is 0 (the comparison in SetMaxThreads); where it is declared is not
	// recovered.
	enum
	{
		kUnlimitedThreads = 0,
	};

	static tThreadPool* Instance();

	void Startup();
	void Shutdown( int32u timeout );
	bool ShutdownComplete();
	void SetMaxThreads( int32u maxThreads );
	int32u GetMaxThreads() const;
	void EmptyWorkQue( bool );
	void Heartbeat();
	void SetMemoryChecking( bool );
	void SetDelayMultiple( float );
	float GetDelayMultiple() const;
	void SetTimeBlockSize( int32u );
	void SetPacketQueLimit( int32u );
	int32u GetTimeBlockSize() const;
	void QueueWorkItem( tCallback<int32>* pCallback, int32 argument );

private:
	tThreadPool();
	~tThreadPool();

	static tThreadPool* fThreadPool;

	// HYPOTHESIS: a queued item is the work item and context QueueWorkItem was given; member
	// names are not recovered.
	class tWork
	{
	public:
		tCallback<int32>*	fpObject;
		int32				fArgument;
	};

	// HYPOTHESIS: member types and names are not recovered, except fRunning (the destructor's
	// check) and the deque of tWork (its instantiations); offsets follow the constructor.
	std::deque<tWork>		fUnknown00;
	int32u					fUnknown14;
	int32u					fUnknown18;
	tPerformanceCounter		fUnknown20;
	float					fUnknown30;
	int32u					fUnknown34;
	int32u					fUnknown38;
	bool					fRunning;
	bool					fUnknown3d;
	bool					fUnknown3e;
	tOptex					fUnknown40;
	int32u					fMaxThreadCount;
	std::list<tThread*>		fUnknown58;
};

#endif
