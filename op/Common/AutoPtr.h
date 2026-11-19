// Common/AutoPtr.h

#if !defined( COMMON_AUTOPTR_H )
#define COMMON_AUTOPTR_H

// PORT: the game was built with the SDK's replacement STL headers, whose auto_ptr assigns
// without a self-assignment test and releases the source only when the pointers differ or the
// source owns; the compiler's own <memory> has the later form. Only what the game's units use is
// declared. A unit that has already included the compiler's own <memory> keeps that one.
#if !defined( _MEMORY_ )
namespace std
{
	template<class _Ty>
	class auto_ptr
	{
	public:
		explicit auto_ptr( _Ty* _P = 0 )
			: _Owns( _P != 0 ), _Ptr( _P )
		{}
		auto_ptr<_Ty>& operator=( const auto_ptr<_Ty>& _Y )
		{
			if( _Ptr != _Y.get() )
			{
				if( _Owns )
					delete _Ptr;
				_Owns = _Y._Owns;
				_Ptr = _Y.release();
			}
			else if( _Y._Owns )
				_Owns = true, _Y.release();
			return *this;
		}
		~auto_ptr()
		{
			if( _Owns )
				delete _Ptr;
		}
		_Ty* get() const
		{
			return _Ptr;
		}
		_Ty* release() const
		{
			((auto_ptr<_Ty>*)this)->_Owns = false;
			return _Ptr;
		}
	private:
		bool _Owns;
		_Ty* _Ptr;
	};
}
#endif

#endif
