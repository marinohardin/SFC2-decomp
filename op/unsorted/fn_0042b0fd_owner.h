#ifndef FN_0042B0FD_OWNER_H
#define FN_0042B0FD_OWNER_H

#include "Common/Database/DatabaseID.h"
#include <xmemory>

// HYPOTHESIS: a container of tIDs laid out like this compiler's std::vector (allocator, first,
// last, end of storage); 0x0042add2 appends one tID. The names are placeholders.
class fn_0042b0fd_owner
{
public:
	void fn_0042b0fd( nDatabase::tID *a, nDatabase::tID *b );
	void fn_0042add2( const nDatabase::tID &id );
	void fn_0053f46d( const nDatabase::tID *first, const nDatabase::tID *last );
	void fn_0053f5d9( nDatabase::tID *pos, const nDatabase::tID *first, const nDatabase::tID *last );
	// MATCH-ONLY: the original read the size through an inlined accessor.
	unsigned int size() const
	{
		return ( m_04 == 0 ? 0 : m_08 - m_04 );
	}
	// MATCH-ONLY: the library's element destroy and uninitialized copy, inlined by the original.
	void _Destroy( nDatabase::tID *first, nDatabase::tID *last )
	{
		for( ; first != last; ++first )
			m_00.destroy( first );
	}
	nDatabase::tID *_Ucopy( const nDatabase::tID *first, const nDatabase::tID *last, nDatabase::tID *dst )
	{
		for( ; first != last; ++dst, ++first )
			m_00.construct( dst, *first );
		return dst;
	}

	std::allocator< nDatabase::tID > m_00;
	nDatabase::tID *m_04;
	nDatabase::tID *m_08;
	nDatabase::tID *m_0c;
};

#endif
