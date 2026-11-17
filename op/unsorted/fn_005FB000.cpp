#include <map>

// The game's out-of-line copy of the library's std::map operator[], for a map of two 4-byte types.
// HYPOTHESIS: key and value are int; the insert it calls (0x0057f946) does not tell 4-byte types apart.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x005fb00a
template<> int& std::map<int, int>::operator[]( const int& _Kv )
{
	iterator _P = insert( value_type( _Kv, int() ) ).first;
	return ( ( *_P ).second );
}

// MATCH-ONLY: the caller of 0x005fb00a is not written yet; VC6 emits the specialization only
// once the class is instantiated. Drop this line when the caller lands.
template class std::map<int, int>;
