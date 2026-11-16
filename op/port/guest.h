// op/port/guest.h: guest mode (DESIGN §7.2). Not original; included only by port/types.h when
// SFCOP_GUEST is defined, never under VC6.
//
// In the native build the original's data lives in the lifted game's 32-bit guest memory: guest
// address A is host address SFCOP_GUEST_BASE + A. Source compiled in guest mode works on that
// memory directly. `this` and every pointer local or parameter is an ordinary host pointer into
// it, and the classes keep the original's layout, so member access needs nothing. What does need
// something is everything whose size or meaning differs between a 32-bit VC6 object and a 64-bit
// clang one; each such construct is below. This header is used only by the port's guest build
// and is never seen by the byte check:
//
//   1. integers           int32 and friends are fixed width (`long` is 64 bits here)
//   2. stored pointers    ptr32( T ): a 32-bit guest address that converts to and from T*
//   3. vtable pointer     SFCOP_VFPTR: a polymorphic class carries the guest vtable pointer as a
//                         32-bit member, and `virtual` is defined away so clang adds no host one
//   4. pure virtuals      SFCOP_PURE: nothing (a non-virtual member cannot be declared pure)
//   5. virtual calls      a call to a virtual member that has no body in guest mode is answered
//                         by a dispatcher that calls through the guest vtable; tools/manifest.py
//                         generates them into the port's override table
//   6. type_info, typeid  ::type_info is the guest's (the VC6 CRT's layout); typeid of an object
//                         asks the guest's own ___RTtypeid, typeid of a pointer expression finds
//                         the guest TypeDescriptor by its decorated name
//   7. std::vector        has the VC6 (Dinkumware) layout: an allocator byte and three pointers,
//                         16 bytes; reading only (growing it would need the guest heap)
//
// The guest runtime the functions here call (sfcop_guest_*) is implemented by the native build
// (the port); op/ declares it and nothing more.
#if !defined( SFCOP_PORT_GUEST_H )
#define SFCOP_PORT_GUEST_H

#if !defined( SFCOP_GUEST_BASE )
#error "guest mode needs SFCOP_GUEST_BASE, the host address of guest address 0"
#endif

// Every host header guest-mode code may need comes first: the macros below (virtual, typeid,
// vector) would break a host header included after them.
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

// 1. integers
typedef int8_t		int8;
typedef uint8_t		int8u;
typedef int16_t		int16;
typedef uint16_t	int16u;
typedef int32_t		int32;
typedef uint32_t	int32u;

extern "C" {
// A call into the guest: the lifted (or overriding) function at `va`, `thisVa` in ecx, `nargs`
// 32-bit arguments pushed right to left. Returns eax.
uint32_t sfcop_guest_call( uint32_t va, uint32_t thisVa, int nargs, const uint32_t* args );
// The guest's ___RTtypeid: the TypeDescriptor of the object's dynamic type.
uint32_t sfcop_guest_typeid( uint32_t objectVa );
// The guest TypeDescriptor whose decorated name is `decorated` (".PAVtFoo@@"), or 0.
uint32_t sfcop_guest_type_descriptor( const char* decorated );
// The guest's type_info::operator==.
int32_t sfcop_guest_type_info_eq( uint32_t a, uint32_t b );
// Something guest mode cannot do; ends the run.
void sfcop_guest_fatal( const char* what );
}

namespace sfcop {
namespace guest {

inline void* host( uint32_t va )
{
	return va ? reinterpret_cast< void* >( static_cast< uintptr_t >( SFCOP_GUEST_BASE ) + va ) : nullptr;
}

inline uint32_t addr( const void* p )
{
	if( !p )
	{
		return 0;
	}
	uintptr_t offset = reinterpret_cast< uintptr_t >( p ) - static_cast< uintptr_t >( SFCOP_GUEST_BASE );
	if( offset >> 32 )
	{
		sfcop_guest_fatal( "a host pointer was stored into guest memory" );
	}
	return static_cast< uint32_t >( offset );
}

// 2. stored pointers
template< class T >
class ptr32
{
public:
	ptr32() = default;
	ptr32( T* p ) : fVa( addr( p ) ) {}
	ptr32( std::nullptr_t ) : fVa( 0 ) {}

	operator T*() const { return static_cast< T* >( host( fVa ) ); }
	T* operator->() const { return *this; }

	uint32_t guestAddress() const { return fVa; }

private:
	uint32_t fVa;
};

static_assert( sizeof( ptr32< int > ) == 4 && alignof( ptr32< int > ) == 4, "a guest pointer is 32 bits" );
static_assert( std::is_trivially_copyable< ptr32< int > >::value, "guest memory is copied bytewise" );

// 3. which classes carry a guest vtable pointer (SFCOP_VFPTR), base classes included
template< class T, class = void >
struct isPolymorphic : std::false_type {};
template< class T >
struct isPolymorphic< T, decltype( (void)T::sfcop_guest_polymorphic ) > : std::true_type {};

}  // namespace guest
}  // namespace sfcop

#define ptr32( T )		::sfcop::guest::ptr32< T >
#define SFCOP_VFPTR		public: static const bool sfcop_guest_polymorphic = true; int32u sfcop_vfptr; private:
#define SFCOP_PURE
#define virtual

// 6. the guest's type_info: the VC6 CRT's layout, its comparison
class type_info
{
public:
	int operator==( const type_info& rhs ) const
	{
		return sfcop_guest_type_info_eq( ::sfcop::guest::addr( this ), ::sfcop::guest::addr( &rhs ) );
	}
	int operator!=( const type_info& rhs ) const { return !( *this == rhs ); }
	const char* raw_name() const { return fDecoratedName; }

	// The CRT's TypeDescriptor: {vftable, spare, decorated name}.
	int32u fVfptr;
	int32u fData;
	char fDecoratedName[ 1 ];
};

namespace sfcop {
namespace guest {

inline const ::type_info& typeInfoAt( uint32_t va )
{
	if( !va )
	{
		sfcop_guest_fatal( "typeid: no guest TypeDescriptor" );
	}
	return *static_cast< const ::type_info* >( host( va ) );
}

// The VC6 decorated name of a pointer to class T (".PAVtFoo@@", ".PBVtFoo@@" for const), from
// clang's spelling of T. Nested classes only: a template or namespace name ends the run.
template< class T >
std::string decoratedPointerName( char classKey )
{
	const char* pretty = __PRETTY_FUNCTION__;
	const char* begin = strstr( pretty, "T = " );
	if( !begin )
	{
		sfcop_guest_fatal( "typeid: cannot read the type's name" );
	}
	begin += 4;
	std::string spelled( begin, strcspn( begin, "];" ) );
	if( spelled.compare( 0, 6, "const " ) == 0 )
	{
		spelled.erase( 0, 6 );
	}
	if( spelled.find_first_of( "<>, " ) != std::string::npos )
	{
		sfcop_guest_fatal( "typeid: only plain and nested class names are decorated in guest mode" );
	}
	std::string decorated = std::is_const< T >::value ? ".PB" : ".PA";
	decorated += classKey;
	size_t end = spelled.size();
	for( ;; )
	{
		size_t colon = spelled.rfind( "::", end == 0 ? 0 : end - 1 );
		size_t start = colon == std::string::npos ? 0 : colon + 2;
		decorated += spelled.substr( start, end - start ) + "@";
		if( colon == std::string::npos )
		{
			break;
		}
		end = colon;
	}
	return decorated + "@";
}

// typeid( object ): the dynamic type, from the guest object's vtable.
template< class T >
const ::type_info& typeidOf( T& object )
{
	static_assert( isPolymorphic< typename std::remove_cv< T >::type >::value,
		"guest-mode typeid of an object needs a class with SFCOP_VFPTR" );
	return typeInfoAt( sfcop_guest_typeid( addr( &object ) ) );
}

// typeid( pointer expression ): the static type T*, looked up once.
template< class T >
const ::type_info& typeidOf( T* const& )
{
	static const uint32_t descriptor = []
	{
		uint32_t va = sfcop_guest_type_descriptor( decoratedPointerName< T >( 'V' ).c_str() );
		return va ? va : sfcop_guest_type_descriptor( decoratedPointerName< T >( 'U' ).c_str() );
	}();
	return typeInfoAt( descriptor );
}

// 7. std::vector in guest memory: VC6's layout and its read-only members.
template< class T >
class vc6Vector
{
public:
	typedef T value_type;
	typedef uint32_t size_type;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T& reference;
	typedef const T& const_reference;

	iterator begin() { return fFirst; }
	const_iterator begin() const { return fFirst; }
	iterator end() { return fLast; }
	const_iterator end() const { return fLast; }
	size_type size() const { return fFirst == nullptr ? 0 : static_cast< size_type >( end() - begin() ); }
	bool empty() const { return size() == 0; }
	reference operator[]( size_type i ) { return *( begin() + i ); }
	const_reference operator[]( size_type i ) const { return *( begin() + i ); }
	reference at( size_type i )
	{
		if( size() <= i )
		{
			sfcop_guest_fatal( "vector::at out of range (a guest throw)" );
		}
		return ( *this )[ i ];
	}

private:
	int8u fAllocator;
	ptr32< T > fFirst;
	ptr32< T > fLast;
	ptr32< T > fEnd;
};

static_assert( sizeof( vc6Vector< int > ) == 16, "VC6's std::vector is 16 bytes" );

}  // namespace guest
}  // namespace sfcop

namespace std {
using ::sfcop::guest::vc6Vector;
}

#define typeid( x )		::sfcop::guest::typeidOf( x )
#define vector			vc6Vector

#endif
