#include <map>
#include <string>
#include <vector>

// The game's out-of-line copy of the library's std::map operator[], for a map of two 4-byte types.
// HYPOTHESIS: key and value are int; the insert it calls (0x0057f946) does not tell 4-byte types apart.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x0042c6da.
// FUNCTION: STARFLEETOP 0x005fb00a
template<> int& std::map<int, int>::operator[]( const int& _Kv )
{
	iterator _P = insert( value_type( _Kv, int() ) ).first;
	return ( ( *_P ).second );
}

// The game's out-of-line copy of the library's tree destructor for the same map.
// HYPOTHESIS: the original's library kept a per-instance nil node at +0x8 and the size at +0x10,
// and freed the nil node here; the toolchain's xtree keeps a static nil, so that tail differs.
// FUNCTION: STARFLEETOP 0x005fb033
template<> std::_Tree<int, std::pair<const int, int>, std::map<int, int>::_Kfn, std::less<int>, std::allocator<int> >::~_Tree()
{
	erase( begin(), end() );
	_Freenode( _Head );
	_Head = 0, _Size = 0;
}

// The game's out-of-line copy of the library's std::map operator[], for a 4-byte key and a vector value.
// HYPOTHESIS: key and element are int; the value's copy constructor (0x0046180d) and destructor
// (0x00402d42) are the vector's, out of line, and do not tell 4-byte element types apart.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb06b
template<> std::vector<int>& std::map<int, std::vector<int> >::operator[]( const int& _Kv )
{
	iterator _P = insert( value_type( _Kv, std::vector<int>() ) ).first;
	return ( ( *_P ).second );
}

// The game's out-of-line copy of the library's tree destructor for the map of vectors.
// HYPOTHESIS: as for 0x005fb033, the original's library freed a per-instance nil node at +0x8.
// FUNCTION: STARFLEETOP 0x005fb0e4
template<> std::_Tree<int, std::pair<const int, std::vector<int> >, std::map<int, std::vector<int> >::_Kfn, std::less<int>, std::allocator<std::vector<int> > >::~_Tree()
{
	erase( begin(), end() );
	_Freenode( _Head );
	_Head = 0, _Size = 0;
}

// The game's out-of-line copy of the library's single-element vector insert, for a 16-byte element.
// HYPOTHESIS: the element is std::string (16 bytes in this library); 0x00404695 is the counted insert.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb124
template<> std::vector<std::string>::iterator std::vector<std::string>::insert( iterator _P, const std::string& _X )
{
	size_type _O = _P - begin();
	insert( _P, 1, _X );
	return ( begin() + _O );
}

// The game's out-of-line copy of the library's tree range erase.
// HYPOTHESIS: the tree is the map of two 4-byte types; the original's library kept a per-instance
// nil node at +0x8, as for 0x005fb033.
// FUNCTION: STARFLEETOP 0x005fb14e
template<> std::_Tree<int, std::pair<const int, int>, std::map<int, int>::_Kfn, std::less<int>, std::allocator<int> >::iterator std::_Tree<int, std::pair<const int, int>, std::map<int, int>::_Kfn, std::less<int>, std::allocator<int> >::erase( iterator _F, iterator _L )
{
	if ( size() == 0 || _F != begin() || _L != end() )
	{
		while ( _F != _L )
			erase( _F++ );
		return ( _F );
	}
	else
	{
		_Erase( _Root() );
		_Root() = _Nil, _Size = 0;
		_Lmost() = _Head, _Rmost() = _Head;
		return ( begin() );
	}
}

// The game's out-of-line copy of the library's std::map insert, for the map of two 4-byte types.
// HYPOTHESIS: the map is the one of 0x005fb00a; 0x0057f946 is its tree's insert.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb1c2
template<> std::map<int, int>::_Pairib std::map<int, int>::insert( const value_type& _X )
{
	_Imp::_Pairib _Ans = _Tr.insert( _X );
	return ( _Pairib( _Ans.first, _Ans.second ) );
}

// The game's out-of-line copy of the library's tree range erase, for another tree.
// HYPOTHESIS: the tree is the map of vectors; 0x005fb3af is its _Erase.
// FUNCTION: STARFLEETOP 0x005fb1e5
template<> std::_Tree<int, std::pair<const int, std::vector<int> >, std::map<int, std::vector<int> >::_Kfn, std::less<int>, std::allocator<std::vector<int> > >::iterator std::_Tree<int, std::pair<const int, std::vector<int> >, std::map<int, std::vector<int> >::_Kfn, std::less<int>, std::allocator<std::vector<int> > >::erase( iterator _F, iterator _L )
{
	if ( size() == 0 || _F != begin() || _L != end() )
	{
		while ( _F != _L )
			erase( _F++ );
		return ( _F );
	}
	else
	{
		_Erase( _Root() );
		_Root() = _Nil, _Size = 0;
		_Lmost() = _Head, _Rmost() = _Head;
		return ( begin() );
	}
}

// MATCH-ONLY: the caller of 0x005fb00a is not written yet; VC6 emits the specialization only
// once the class is instantiated. Drop this line when the caller lands.
template class std::map<int, int>;
template class std::map<int, std::vector<int> >;
template class std::vector<std::string>;
