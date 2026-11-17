// HYPOTHESIS: the owning class of these 7-byte member-address getters is not known yet;
// the getters return the address of members at +0x340, +0x3b4, +0x4e8, +0x260, +0x270 and +0x1e4.
class fn_0057a481_object {
public:
    char *fn_0057a481();
    char *fn_0057a488();
    char *fn_0057a48f();
    char *fn_0057a496();
    char *fn_0057a49d();
    char *fn_0057a4a4();

    char field_0[0x1e4];
    char field_1e4;
    char field_1e5[0x260 - 0x1e5];
    char field_260;
    char field_261[0x270 - 0x261];
    char field_270;
    char field_271[0x340 - 0x271];
    char field_340;
    char field_341[0x3b4 - 0x341];
    char field_3b4;
    char field_3b5[0x4e8 - 0x3b5];
    char field_4e8;
};

// FUNCTION: STARFLEETOP 0x0057a481
char *fn_0057a481_object::fn_0057a481()
{
    return &field_340;
}

// FUNCTION: STARFLEETOP 0x0057a488
char *fn_0057a481_object::fn_0057a488()
{
    return &field_3b4;
}

// FUNCTION: STARFLEETOP 0x0057a48f
char *fn_0057a481_object::fn_0057a48f()
{
    return &field_4e8;
}

// FUNCTION: STARFLEETOP 0x0057a496
char *fn_0057a481_object::fn_0057a496()
{
    return &field_260;
}

// FUNCTION: STARFLEETOP 0x0057a49d
char *fn_0057a481_object::fn_0057a49d()
{
    return &field_270;
}

// FUNCTION: STARFLEETOP 0x0057a4a4
char *fn_0057a481_object::fn_0057a4a4()
{
    return &field_1e4;
}

// HYPOTHESIS: 0x0057a577 is the tail of a larger stream function that shares its parent frame; the parent is not rebuilt yet.
// FUNCTION: STARFLEETOP 0x0057a577
void *fn_0057a577()
{
    return 0;
}

// HYPOTHESIS: 0x0057a5be is a list's clear: member +4 is the sentinel node, +8 the count;
// the node callee and the deallocator are not identified yet.
struct fn_0057a5be_node {
    fn_0057a5be_node *next;
};

struct fn_0057a5be_alloc {
    int field_0;
};

void fn_00583342(fn_0057a5be_alloc *alloc, fn_0057a5be_node *node);

class fn_0057a5be_object {
public:
    void fn_0057a5be();

    int field_0;
    fn_0057a5be_node *field_4;
    int field_8;
};

// FUNCTION: STARFLEETOP 0x0057a5be
void fn_0057a5be_object::fn_0057a5be()
{
    fn_0057a5be_alloc alloc;
    fn_0057a5be_node *head = field_4;
    fn_0057a5be_node *node = head->next;
    while (node != head) {
        fn_0057a5be_node *cur = node;
        node = node->next;
        fn_00583342(&alloc, cur);
    }
    delete field_4;
    field_4 = 0;
    field_8 = 0;
}

// HYPOTHESIS: 0x0057a5f6 is a list's assignment operator in the standard library's shape
// (node: next, prev, value at +8); the element type, whose assignment is 0x005b63e9, is not
// identified yet.
class fn_0057a5f6_value {
public:
    fn_0057a5f6_value &operator=(const fn_0057a5f6_value &other);
};

struct fn_0057a5f6_node {
    fn_0057a5f6_node *next;
    fn_0057a5f6_node *prev;
    fn_0057a5f6_value value;
};

class fn_0057a5f6_iterator {
public:
    fn_0057a5f6_iterator(fn_0057a5f6_node *p) : ptr(p) {}
    fn_0057a5f6_value &operator*() const { return ptr->value; }
    fn_0057a5f6_iterator &operator++() { ptr = ptr->next; return *this; }
    fn_0057a5f6_iterator operator++(int) { fn_0057a5f6_iterator tmp = *this; ++*this; return tmp; }
    bool operator==(const fn_0057a5f6_iterator &other) const { return ptr == other.ptr; }
    bool operator!=(const fn_0057a5f6_iterator &other) const { return !(*this == other); }

    fn_0057a5f6_node *ptr;
};

class fn_0057a5f6_list {
public:
    fn_0057a5f6_list &operator=(const fn_0057a5f6_list &other);
    fn_0057a5f6_iterator begin() const { return fn_0057a5f6_iterator(field_4->next); }
    fn_0057a5f6_iterator end() const { return fn_0057a5f6_iterator(field_4); }
    fn_0057a5f6_iterator erase(fn_0057a5f6_iterator where);
    fn_0057a5f6_iterator insert(fn_0057a5f6_iterator where, const fn_0057a5f6_value &value);
    void insert(fn_0057a5f6_iterator where, fn_0057a5f6_iterator first, fn_0057a5f6_iterator last)
        { for (; first != last; ++first) insert(where, *first); }
    fn_0057a5f6_iterator erase(fn_0057a5f6_iterator first, fn_0057a5f6_iterator last)
        { while (first != last) erase(first++); return first; }

    int field_0;
    fn_0057a5f6_node *field_4;
    int field_8;
};

// FUNCTION: STARFLEETOP 0x0057a5f6
fn_0057a5f6_list &fn_0057a5f6_list::operator=(const fn_0057a5f6_list &other)
{
    if (this != &other) {
        fn_0057a5f6_iterator first1 = begin();
        fn_0057a5f6_iterator last1 = end();
        fn_0057a5f6_iterator first2 = other.begin();
        fn_0057a5f6_iterator last2 = other.end();
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            *first1 = *first2;
        erase(first1, last1);
        insert(last1, first2, last2);
    }
    return *this;
}

// HYPOTHESIS: 0x0057a66b is a vector's tidy (first +4, last +8, end +0xc) over 0x4c-byte
// elements whose destructor is 0x00579ad8; the element type is not identified yet. The element
// is a 0x48-byte part whose assignment is 0x00716ff0, then a dword at +0x48 (from 0x0057a6af).
class fn_0057a66b_part {
public:
    fn_0057a66b_part(const fn_0057a66b_part &other);
    fn_0057a66b_part &operator=(const fn_0057a66b_part &other);

    char field_0[0x48];
};

class fn_0057a66b_value {
public:
    ~fn_0057a66b_value();

    fn_0057a66b_part field_0;
    int field_48;
};

class fn_0057a66b_vector {
public:
    void fn_0057a66b();
    fn_0057a66b_value *fn_0057a6af(fn_0057a66b_value *first, fn_0057a66b_value *last);

    int field_0;
    fn_0057a66b_value *field_4;
    fn_0057a66b_value *field_8;
    fn_0057a66b_value *field_c;
};

// FUNCTION: STARFLEETOP 0x0057a66b
void fn_0057a66b_vector::fn_0057a66b()
{
    fn_0057a66b_value *last = field_8;
    for (fn_0057a66b_value *p = field_4; p != last; ++p)
        p->~fn_0057a66b_value();
    operator delete(field_4);
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
}

// HYPOTHESIS: 0x0057a69e is a vector's push_back (insert(end(), 1, x)); last is at +8 and
// 0x0057a716 is the vector's insert(iterator, n, value). The element type is not identified.
typedef fn_0057a66b_value fn_0057a69e_value;

class fn_0057a69e_vector {
public:
    void fn_0057a69e(const fn_0057a69e_value &value);
    void fn_0057a716(fn_0057a69e_value *where, unsigned int count, const fn_0057a69e_value &value);
    unsigned int size() const { return field_4 == 0 ? 0 : field_8 - field_4; }

    int field_0;
    fn_0057a69e_value *field_4;
    fn_0057a69e_value *field_8;
    fn_0057a69e_value *field_c;
};

// FUNCTION: STARFLEETOP 0x0057a69e
void fn_0057a69e_vector::fn_0057a69e(const fn_0057a69e_value &value)
{
    fn_0057a716(field_8, 1, value);
}

#include <algorithm>

// HYPOTHESIS: 0x0057a6af is the same vector's erase(first, last).
// FUNCTION: STARFLEETOP 0x0057a6af
fn_0057a66b_value *fn_0057a66b_vector::fn_0057a6af(fn_0057a66b_value *first, fn_0057a66b_value *last)
{
    fn_0057a66b_value *dest = std::copy(last, field_8, first);
    fn_0057a66b_value *old = field_8;
    for (fn_0057a66b_value *p = dest; p != old; ++p)
        p->~fn_0057a66b_value();
    field_8 = dest;
    return first;
}

#include <memory>

// HYPOTHESIS: 0x0057a716 is the vector's insert(where, count, value) in the standard
// library's shape; the element is the 0x4c-byte value of 0x0057a66b, copy-constructed by
// 0x0057ab18.
// FUNCTION: STARFLEETOP 0x0057a716
void fn_0057a69e_vector::fn_0057a716(fn_0057a69e_value *where, unsigned int count, const fn_0057a69e_value &value)
{
    if ((unsigned int)(field_c - field_8) < count) {
        unsigned int n = size() + (count < size() ? size() : count);
        fn_0057a69e_value *s = std::_Allocate((int)n, (fn_0057a69e_value *)0);
        fn_0057a69e_value *q = std::uninitialized_copy(field_4, where, s);
        std::uninitialized_fill_n(q, count, value);
        std::uninitialized_copy(where, field_8, q + count);
        fn_0057a69e_value *last = field_8;
        for (fn_0057a69e_value *p = field_4; p != last; ++p)
            std::_Destroy(p);
        operator delete(field_4);
        field_c = s + n;
        field_8 = s + size() + count;
        field_4 = s;
    } else if ((unsigned int)(field_8 - where) < count) {
        std::uninitialized_copy(where, field_8, where + count);
        std::uninitialized_fill_n(field_8, count - (field_8 - where), value);
        std::fill(where, field_8, value);
        field_8 += count;
    } else if (0 < count) {
        std::uninitialized_copy(field_8 - count, field_8, field_8);
        std::copy_backward(where, field_8 - count, field_8);
        std::fill(where, where + count, value);
        field_8 += count;
    }
}

#include "Common/SmartPtr.h"

// HYPOTHESIS: the analysis names 0x0057a96e
// tMemberCallback<nAsyncMissionProcedures::tChooseMission,nSwitch::tAddress::tStatusChangeInfo>
// slot 2, the SDK template's operator(). tCallback<> is declared only inside
// Common/Thread/ThreadPool.cpp, and neither tChooseMission nor tAddress is declared yet, so
// the base is left out and both classes stand in as fn_0057a96e_* types here.
// tStatusChangeInfo is 12 bytes: a smart pointer to an object whose tRefCount sits at +0xc,
// then two dwords.
class fn_0057a96e_base {
public:
    virtual ~fn_0057a96e_base();

    int field_4;
    int field_8;
};

class fn_0057a96e_target : public fn_0057a96e_base, public tRefCount {
};

struct fn_0057a96e_info {
    // HYPOTHESIS: a tConstSmartPtr; a tSmartPtr leaves the generated copy out of line.
    tConstSmartPtr<fn_0057a96e_target> field_0;
    int field_4;
    int field_8;
};

class fn_0057a96e_owner {
};

#include "Common/Callback.h"

template< typename tnMemberClass, typename tnResult >
class tMemberCallback
	: public tCallback<tnResult>
{
public:
	typedef void (tnMemberClass::*iMemberFunction)( tnResult result );

	tMemberCallback( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction );
	virtual ~tMemberCallback();

	virtual void Callback( tnResult result );
	virtual void operator()( tnResult result );

protected:
	tnMemberClass* fpMemberClass;
	iMemberFunction fpMemberFunction;
};

// FUNCTION: STARFLEETOP 0x0057a96e
template< typename tnMemberClass, typename tnResult >
void tMemberCallback<tnMemberClass,tnResult>::operator()( tnResult result )
{
	(fpMemberClass->*fpMemberFunction)( result );
}

// HYPOTHESIS: slot 3 is the SDK template's other virtual with the same body; which of
// Callback and operator() sits at which address is not settled by the bytes.
// FUNCTION: STARFLEETOP 0x0057a9ce
template< typename tnMemberClass, typename tnResult >
void tMemberCallback<tnMemberClass,tnResult>::Callback( tnResult result )
{
	(fpMemberClass->*fpMemberFunction)( result );
}

template class tMemberCallback<fn_0057a96e_owner, fn_0057a96e_info>;

#include <string>

namespace std {

// HYPOTHESIS: the exe's copy is the library's operator+(const char*, const string&); it is
// written out here so the claim has a definition in this unit.
// FUNCTION: STARFLEETOP 0x0057aa2e
string operator+( const char* _L, const string& _R )
{
	// MATCH-ONLY: the exe passes the literal -1 where this library header's default is npos.
	return string( _L ).append( _R, 0, (size_t)-1 );
}
}

namespace nDatabase {
class tID;
}

template< typename tnMemberClass, typename tnResult >
inline tMemberCallback<tnMemberClass,tnResult>::tMemberCallback( tnMemberClass* pMemberClass, iMemberFunction pMemberFunction ) :
	fpMemberClass( pMemberClass ),
	fpMemberFunction( pMemberFunction )
{}

// HYPOTHESIS: the analysis names 0x0057aab3 the tMemberCallback<tChooseMission,const tID&>
// constructor, but the object comes in as a hidden return pointer: the SDK's CreateCallback
// helper with the constructor inlined. fn_0057a96e_owner stands in for tChooseMission.
// FUNCTION: STARFLEETOP 0x0057aab3
template< typename tParam, typename tClass >
tMemberCallback< tClass, tParam > CreateCallback( tClass* object, void (tClass::*func)( tParam result ) )
{
	return 	tMemberCallback< tClass, tParam >( object, func );
}

template tMemberCallback< fn_0057a96e_owner, const nDatabase::tID& > CreateCallback( fn_0057a96e_owner* object, void (fn_0057a96e_owner::*func)( const nDatabase::tID& result ) );

// FUNCTION: STARFLEETOP 0x0057aad2
template< typename tnResult >
inline tCallback<tnResult>::~tCallback()
{}

template< typename tnMemberClass, typename tnResult >
inline tMemberCallback<tnMemberClass,tnResult>::~tMemberCallback()
{}

template class tCallback< const nDatabase::tID& >;

#include "Common/Database/DatabaseID.h"

// HYPOTHESIS: the analysis names 0x0057aad9 a tCallback constructor, but it is a free function
// that hands a range's two iterators and a copy of the callback to std::for_each (0x0057abdb).
// The range type is not identified; a pair of tID pointers stands in for it.
// FUNCTION: STARFLEETOP 0x0057aad9
template< typename tnRange, typename tnFunction >
void fn_0057aad9( const tnRange& range, const tnFunction& function )
{
	std::for_each( range.first, range.second, function );
}

template void fn_0057aad9( const std::pair< const nDatabase::tID*, const nDatabase::tID* >& range, const tMemberCallback< fn_0057a96e_owner, const nDatabase::tID& >& function );

// HYPOTHESIS: 0x0057ab18 is the library's _Construct for the 0x4c-byte element of 0x0057a66b,
// held out of line; it copies the 0x48-byte part through 0x00716d70, then the dword at +0x48.
// MATCH-ONLY: written as a plain function, since an explicit specialization of std::_Construct
// here never finishes compiling under the original compiler.
// FUNCTION: STARFLEETOP 0x0057ab18
void fn_0057ab18( fn_0057a69e_value* _P, const fn_0057a69e_value& _V )
{
	new ((void*)_P) fn_0057a69e_value(_V);
}
