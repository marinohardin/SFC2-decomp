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

// The game's out-of-line copy of the library's tree range erase, for a third tree.
// HYPOTHESIS: the tree's type is not known; a map of int to string stands in. 0x005fb6e5 is its _Erase,
// 0x005fb47a its single-element erase.
// FUNCTION: STARFLEETOP 0x005fb259
template<> std::_Tree<int, std::pair<const int, std::string>, std::map<int, std::string>::_Kfn, std::less<int>, std::allocator<std::string> >::iterator std::_Tree<int, std::pair<const int, std::string>, std::map<int, std::string>::_Kfn, std::less<int>, std::allocator<std::string> >::erase( iterator _F, iterator _L )
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

// The game's out-of-line copy of the library's tree insert, for a tree keyed by a string.
// HYPOTHESIS: the key is std::string (0x0040651e is its compare) and the mapped type, which this
// function does not show, is int. 0x0042dcfb is the iterator's decrement, 0x005fb71d its _Insert.
// The original's library kept a per-instance nil node at +0x8 and _Multi at +0xc, as for 0x005fb033;
// the toolchain's xtree keeps a static nil, so the loop test and the _Multi test differ.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb2cd
template<> std::_Tree<std::string, std::pair<const std::string, int>, std::map<std::string, int>::_Kfn, std::less<std::string>, std::allocator<int> >::_Pairib std::_Tree<std::string, std::pair<const std::string, int>, std::map<std::string, int>::_Kfn, std::less<std::string>, std::allocator<int> >::insert( const value_type& _V )
{
	_Nodeptr _X = _Root();
	_Nodeptr _Y = _Head;
	bool _Ans = true;
	while ( _X != _Nil )
	{
		_Y = _X;
		_Ans = key_compare( _Kfn()( _V ), _Key( _X ) );
		_X = _Ans ? _Left( _X ) : _Right( _X );
	}
	if ( _Multi )
		return ( _Pairib( _Insert( _X, _Y, _V ), true ) );
	iterator _P = iterator( _Y );
	if ( !_Ans )
		;
	else if ( _P == begin() )
		return ( _Pairib( _Insert( _X, _Y, _V ), true ) );
	else
		--_P;
	if ( key_compare( _Key( _P._Mynode() ), _Kfn()( _V ) ) )
		return ( _Pairib( _Insert( _X, _Y, _V ), true ) );
	return ( _Pairib( _P, false ) );
}

// The game's out-of-line copy of the library's recursive tree erase, for the tree keyed by a string.
// HYPOTHESIS: the tree is the one of 0x005fb2cd; the value's destructor (0x005f762d) only frees the
// key string, so the linker shares it with another type's. The original's library kept a
// per-instance nil node at +0x8, as for 0x005fb033.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb377
template<> void std::_Tree<std::string, std::pair<const std::string, int>, std::map<std::string, int>::_Kfn, std::less<std::string>, std::allocator<int> >::_Erase( _Nodeptr _X )
{
	for ( _Nodeptr _Y = _X; _Y != _Nil; _X = _Y )
	{
		_Erase( _Right( _Y ) );
		_Y = _Left( _Y );
		_Destval( &_Value( _X ) );
		_Freenode( _X );
	}
}

// The game's out-of-line copy of the library's recursive tree erase, for a tree with a trivial value.
// HYPOTHESIS: the tree is the map of two 4-byte types of 0x005fb14e; the value needs no destructor.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb3af
template<> void std::_Tree<int, std::pair<const int, int>, std::map<int, int>::_Kfn, std::less<int>, std::allocator<int> >::_Erase( _Nodeptr _X )
{
	// MATCH-ONLY: the nil node is read from the object (+0x8), where the original's library kept it.
	for ( _Nodeptr _Y = _X; _Y != *(_Nodeptr*)( (char*)this + 8 ); _X = _Y )
	{
		_Erase( _Right( _Y ) );
		_Y = _Left( _Y );
		_Destval( &_Value( _X ) );
		_Freenode( _X );
	}
}

// The game's out-of-line copy of the library's tree insert, for a tree keyed by a 4-byte integer.
// HYPOTHESIS: the tree is the map of vectors of 0x005fb1e5 (the map of two 4-byte types inserts through
// 0x0057f946). 0x0042dcfb is the iterator's decrement, 0x005fb867 its _Insert.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb3df
template<> std::_Tree<int, std::pair<const int, std::vector<int> >, std::map<int, std::vector<int> >::_Kfn, std::less<int>, std::allocator<std::vector<int> > >::_Pairib std::_Tree<int, std::pair<const int, std::vector<int> >, std::map<int, std::vector<int> >::_Kfn, std::less<int>, std::allocator<std::vector<int> > >::insert( const value_type& _V )
{
	_Nodeptr _X = _Root();
	_Nodeptr _Y = _Head;
	bool _Ans = true;
	// MATCH-ONLY: the nil node and _Multi are read from the object (+0x8, +0xc), where the original's library kept them.
	while ( _X != *(_Nodeptr*)( (char*)this + 8 ) )
	{
		_Y = _X;
		_Ans = key_compare( _Kfn()( _V ), _Key( _X ) );
		_X = _Ans ? _Left( _X ) : _Right( _X );
	}
	if ( *( (char*)this + 0xc ) )
		return ( _Pairib( _Insert( _X, _Y, _V ), true ) );
	iterator _P = iterator( _Y );
	if ( !_Ans )
		;
	else if ( _P == begin() )
		return ( _Pairib( _Insert( _X, _Y, _V ), true ) );
	else
		--_P;
	if ( key_compare( _Key( _P._Mynode() ), _Kfn()( _V ) ) )
		return ( _Pairib( _Insert( _X, _Y, _V ), true ) );
	return ( _Pairib( _P, false ) );
}

// MATCH-ONLY: the caller of 0x005fb00a is not written yet; VC6 emits the specialization only
// once the class is instantiated. Drop this line when the caller lands.
template class std::map<int, int>;
template class std::map<int, std::vector<int> >;
template class std::vector<std::string>;
template class std::map<int, std::string>;
template class std::map<std::string, int>;
