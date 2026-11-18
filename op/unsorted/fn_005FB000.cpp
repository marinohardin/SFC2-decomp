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

// MATCH-ONLY: the original's library's _Min and _Max, which stop at a node's nil flag (+0x21);
// the toolchain's compare with a static nil node instead. Left is at +0x0, right at +0x8.
template<class _Np> inline _Np fn_TreeMin( _Np _P )
{
	while ( !*( (char*)*(_Np*)_P + 0x21 ) )
		_P = *(_Np*)_P;
	return ( _P );
}

template<class _Np> inline _Np fn_TreeMax( _Np _P )
{
	while ( !*( (char*)*( (_Np*)_P + 2 ) + 0x21 ) )
		_P = *( (_Np*)_P + 2 );
	return ( _P );
}

// The game's out-of-line copy of the library's single-element tree erase, for the tree of 0x005fb259.
// HYPOTHESIS: the tree is the stand-in map of int to string; 0x005fb11c is its _Destval, 0x00556fd0
// and 0x00558526 the rotations, 0x0052aad3 the iterator's increment.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb47a
template<> std::_Tree<int, std::pair<const int, std::string>, std::map<int, std::string>::_Kfn, std::less<int>, std::allocator<std::string> >::iterator std::_Tree<int, std::pair<const int, std::string>, std::map<int, std::string>::_Kfn, std::less<int>, std::allocator<std::string> >::erase( iterator _P )
{
	// MATCH-ONLY: the nil node is read from the object (+0x8) and a node's nil flag from +0x21,
	// where the original's library kept them; the toolchain's xtree has neither.
	_Nodeptr _X;
	_Nodeptr _Y = ( _P++ )._Mynode();
	_Nodeptr _Z = _Y;
	if ( _Left( _Y ) == *(_Nodeptr*)( (char*)this + 8 ) )
		_X = _Right( _Y );
	else if ( _Right( _Y ) == *(_Nodeptr*)( (char*)this + 8 ) )
		_X = _Left( _Y );
	else
		_Y = fn_TreeMin( _Right( _Y ) ), _X = _Right( _Y );
	if ( _Y != _Z )
	{
		_Parent( _Left( _Z ) ) = _Y;
		_Left( _Y ) = _Left( _Z );
		if ( _Y == _Right( _Z ) )
			_Parent( _X ) = _Y;
		else
		{
			_Parent( _X ) = _Parent( _Y );
			_Left( _Parent( _Y ) ) = _X;
			_Right( _Y ) = _Right( _Z );
			_Parent( _Right( _Z ) ) = _Y;
		}
		if ( _Root() == _Z )
			_Root() = _Y;
		else if ( _Left( _Parent( _Z ) ) == _Z )
			_Left( _Parent( _Z ) ) = _Y;
		else
			_Right( _Parent( _Z ) ) = _Y;
		_Parent( _Y ) = _Parent( _Z );
		std::swap( *( (char*)_Y + 0x20 ), *( (char*)_Z + 0x20 ) );
		_Y = _Z;
	}
	else
	{
		_Parent( _X ) = _Parent( _Y );
		if ( _Root() == _Z )
			_Root() = _X;
		else if ( _Left( _Parent( _Z ) ) == _Z )
			_Left( _Parent( _Z ) ) = _X;
		else
			_Right( _Parent( _Z ) ) = _X;
		if ( _Lmost() != _Z )
			;
		else if ( _Right( _Z ) == *(_Nodeptr*)( (char*)this + 8 ) )
			_Lmost() = _Parent( _Z );
		else
			_Lmost() = fn_TreeMin( _X );
		if ( _Rmost() != _Z )
			;
		else if ( _Left( _Z ) == *(_Nodeptr*)( (char*)this + 8 ) )
			_Rmost() = _Parent( _Z );
		else
			_Rmost() = fn_TreeMax( _X );
	}
	// MATCH-ONLY: the colour is a byte at +0x20 (red 0, black 1) and the size sits at +0x10 in the
	// original's library.
	if ( *( (char*)_Y + 0x20 ) == 1 )
	{
		for ( ; _X != _Root() && *( (char*)_X + 0x20 ) == 1; _X = _Parent( _X ) )
			if ( _X == _Left( _Parent( _X ) ) )
			{
				_Nodeptr _W = _Right( _Parent( _X ) );
				if ( *( (char*)_W + 0x20 ) == 0 )
				{
					*( (char*)_W + 0x20 ) = 1;
					*( (char*)_Parent( _X ) + 0x20 ) = 0;
					_Lrotate( _Parent( _X ) );
					_W = _Right( _Parent( _X ) );
				}
				if ( *( (char*)_Left( _W ) + 0x20 ) == 1 && *( (char*)_Right( _W ) + 0x20 ) == 1 )
				{
					*( (char*)_W + 0x20 ) = 0;
					continue;
				}
				else if ( *( (char*)_Right( _W ) + 0x20 ) == 1 )
				{
					*( (char*)_Left( _W ) + 0x20 ) = 1;
					*( (char*)_W + 0x20 ) = 0;
					_Rrotate( _W );
					_W = _Right( _Parent( _X ) );
				}
				*( (char*)_W + 0x20 ) = *( (char*)_Parent( _X ) + 0x20 );
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Right( _W ) + 0x20 ) = 1;
				_Lrotate( _Parent( _X ) );
				break;
			}
			else
			{
				_Nodeptr _W = _Left( _Parent( _X ) );
				if ( *( (char*)_W + 0x20 ) == 0 )
				{
					*( (char*)_W + 0x20 ) = 1;
					*( (char*)_Parent( _X ) + 0x20 ) = 0;
					_Rrotate( _Parent( _X ) );
					_W = _Left( _Parent( _X ) );
				}
				if ( *( (char*)_Right( _W ) + 0x20 ) == 1 && *( (char*)_Left( _W ) + 0x20 ) == 1 )
				{
					*( (char*)_W + 0x20 ) = 0;
					continue;
				}
				else if ( *( (char*)_Left( _W ) + 0x20 ) == 1 )
				{
					*( (char*)_Right( _W ) + 0x20 ) = 1;
					*( (char*)_W + 0x20 ) = 0;
					_Lrotate( _W );
					_W = _Left( _Parent( _X ) );
				}
				*( (char*)_W + 0x20 ) = *( (char*)_Parent( _X ) + 0x20 );
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Left( _W ) + 0x20 ) = 1;
				_Rrotate( _Parent( _X ) );
				break;
			}
		*( (char*)_X + 0x20 ) = 1;
	}
	_Destval( &_Value( _Y ) );
	_Freenode( _Y );
	--*(size_type*)( (char*)this + 0x10 );
	return ( _P );
}

// The game's out-of-line copy of the library's recursive tree erase, for the tree of 0x005fb259.
// HYPOTHESIS: the tree is the stand-in map of int to string; 0x005fb11c is its _Destval.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb6e5
template<> void std::_Tree<int, std::pair<const int, std::string>, std::map<int, std::string>::_Kfn, std::less<int>, std::allocator<std::string> >::_Erase( _Nodeptr _X )
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

// The game's out-of-line copy of the library's tree _Insert, for the tree keyed by a string.
// HYPOTHESIS: the tree is the one of 0x005fb2cd; 0x005fba9d is the value's copy constructor,
// 0x00556fd0 and 0x00558526 the rotations.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb71d
template<> std::_Tree<std::string, std::pair<const std::string, int>, std::map<std::string, int>::_Kfn, std::less<std::string>, std::allocator<int> >::iterator std::_Tree<std::string, std::pair<const std::string, int>, std::map<std::string, int>::_Kfn, std::less<std::string>, std::allocator<int> >::_Insert( _Nodeptr _X, _Nodeptr _Y, const _Ty& _V )
{
	// MATCH-ONLY: the original's library built the node itself: colour byte at +0x20, nil flag at
	// +0x21, a per-instance nil node at +0x8 and the size at +0x10.
	_Nodeptr _Z = (_Nodeptr)allocator._Charalloc( 1 * sizeof ( _Node ) );
	*( (char*)_Z + 0x20 ) = 0;
	*( (char*)_Z + 0x21 ) = 0;
	_Parent( _Z ) = _Y;
	_Left( _Z ) = *(_Nodeptr*)( (char*)this + 8 );
	_Right( _Z ) = *(_Nodeptr*)( (char*)this + 8 );
	_Consval( &_Value( _Z ), _V );
	++*(size_type*)( (char*)this + 0x10 );
	if ( _Y == _Head || _X != *(_Nodeptr*)( (char*)this + 8 ) || key_compare( _Kfn()( _V ), _Key( _Y ) ) )
	{
		_Left( _Y ) = _Z;
		if ( _Y == _Head )
		{
			_Root() = _Z;
			_Rmost() = _Z;
		}
		else if ( _Y == _Lmost() )
			_Lmost() = _Z;
	}
	else
	{
		_Right( _Y ) = _Z;
		if ( _Y == _Rmost() )
			_Rmost() = _Z;
	}
	for ( _X = _Z; _X != _Root() && *( (char*)_Parent( _X ) + 0x20 ) == 0; )
		if ( _Parent( _X ) == _Left( _Parent( _Parent( _X ) ) ) )
		{
			_Y = _Right( _Parent( _Parent( _X ) ) );
			if ( *( (char*)_Y + 0x20 ) == 0 )
			{
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Y + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_X = _Parent( _Parent( _X ) );
			}
			else
			{
				if ( _X == _Right( _Parent( _X ) ) )
				{
					_X = _Parent( _X );
					_Lrotate( _X );
				}
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_Rrotate( _Parent( _Parent( _X ) ) );
			}
		}
		else
		{
			_Y = _Left( _Parent( _Parent( _X ) ) );
			if ( *( (char*)_Y + 0x20 ) == 0 )
			{
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Y + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_X = _Parent( _Parent( _X ) );
			}
			else
			{
				if ( _X == _Left( _Parent( _X ) ) )
				{
					_X = _Parent( _X );
					_Rrotate( _X );
				}
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_Lrotate( _Parent( _Parent( _X ) ) );
			}
		}
	*( (char*)_Root() + 0x20 ) = 1;
	return ( iterator( _Z ) );
}

// The game's out-of-line copy of the library's tree _Insert, for the tree of 0x005fb3df.
// HYPOTHESIS: the tree is the map of vectors of 0x005fb1e5; 0x005fbaca is the value's copy
// constructor, 0x00556fd0 and 0x00558526 the rotations.
// MATCH-ONLY: written as a specialization so the copy's body sits in this unit, as for 0x005fb00a.
// FUNCTION: STARFLEETOP 0x005fb867
template<> std::_Tree<int, std::pair<const int, std::vector<int> >, std::map<int, std::vector<int> >::_Kfn, std::less<int>, std::allocator<std::vector<int> > >::iterator std::_Tree<int, std::pair<const int, std::vector<int> >, std::map<int, std::vector<int> >::_Kfn, std::less<int>, std::allocator<std::vector<int> > >::_Insert( _Nodeptr _X, _Nodeptr _Y, const _Ty& _V )
{
	// MATCH-ONLY: the original's library built the node itself: colour byte at +0x20, nil flag at
	// +0x21, a per-instance nil node at +0x8 and the size at +0x10.
	_Nodeptr _Z = (_Nodeptr)allocator._Charalloc( 1 * sizeof ( _Node ) );
	*( (char*)_Z + 0x20 ) = 0;
	*( (char*)_Z + 0x21 ) = 0;
	_Parent( _Z ) = _Y;
	_Left( _Z ) = *(_Nodeptr*)( (char*)this + 8 );
	_Right( _Z ) = *(_Nodeptr*)( (char*)this + 8 );
	_Consval( &_Value( _Z ), _V );
	++*(size_type*)( (char*)this + 0x10 );
	if ( _Y == _Head || _X != *(_Nodeptr*)( (char*)this + 8 ) || key_compare( _Kfn()( _V ), _Key( _Y ) ) )
	{
		_Left( _Y ) = _Z;
		if ( _Y == _Head )
		{
			_Root() = _Z;
			_Rmost() = _Z;
		}
		else if ( _Y == _Lmost() )
			_Lmost() = _Z;
	}
	else
	{
		_Right( _Y ) = _Z;
		if ( _Y == _Rmost() )
			_Rmost() = _Z;
	}
	for ( _X = _Z; _X != _Root() && *( (char*)_Parent( _X ) + 0x20 ) == 0; )
		if ( _Parent( _X ) == _Left( _Parent( _Parent( _X ) ) ) )
		{
			_Y = _Right( _Parent( _Parent( _X ) ) );
			if ( *( (char*)_Y + 0x20 ) == 0 )
			{
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Y + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_X = _Parent( _Parent( _X ) );
			}
			else
			{
				if ( _X == _Right( _Parent( _X ) ) )
				{
					_X = _Parent( _X );
					_Lrotate( _X );
				}
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_Rrotate( _Parent( _Parent( _X ) ) );
			}
		}
		else
		{
			_Y = _Left( _Parent( _Parent( _X ) ) );
			if ( *( (char*)_Y + 0x20 ) == 0 )
			{
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Y + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_X = _Parent( _Parent( _X ) );
			}
			else
			{
				if ( _X == _Left( _Parent( _X ) ) )
				{
					_X = _Parent( _X );
					_Rrotate( _X );
				}
				*( (char*)_Parent( _X ) + 0x20 ) = 1;
				*( (char*)_Parent( _Parent( _X ) ) + 0x20 ) = 0;
				_Lrotate( _Parent( _Parent( _X ) ) );
			}
		}
	*( (char*)_Root() + 0x20 ) = 1;
	return ( iterator( _Z ) );
}

#include "Common/MemberCallback.h"
#include "unsorted/tMetaClientChatCtlrC.h"

class tChatChannel;

// MATCH-ONLY: the SDK's template CreateCallback, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// FUNCTION: STARFLEETOP 0x005fb9ab
template<>
tMemberCallback< tMetaClientChatCtlrC, const tChatChannel& > CreateCallback( tMetaClientChatCtlrC* object, void (tMetaClientChatCtlrC::*func)( const tChatChannel& result ) )
{
	return tMemberCallback< tMetaClientChatCtlrC, const tChatChannel& >( object, func );
}

#include "Common/Callback.h"

// MATCH-ONLY: the SDK's template destructor, written out as this instance's inline explicit
// specialization so that its annotation names the one instance. Its body (storing the base
// vtable) is the same as ~tCallback's, which the analysis names at this address.
// FUNCTION: STARFLEETOP 0x005fbb1f
// FUNCTION: STARFLEETOP 0x005fb9ca
template<>
inline tMemberCallback< tMetaClientChatCtlrC, const tChatChannel& >::~tMemberCallback()
{}

#include <algorithm>
#include <utility>
#include "unsorted/tChatChannel.h"

// HYPOTHESIS: the first argument holds the first and past-the-end channel at +0 and +4; it is
// written as a pair of pointers, the toolchain's vector keeping its first element at +4.
// FUNCTION: STARFLEETOP 0x005fb9d1
void FUN_005fb9d1( const std::pair<const tChatChannel*, const tChatChannel*>& channels, const tMemberCallback< tMetaClientChatCtlrC, const tChatChannel& >& callback )
{
	std::for_each( channels.first, channels.second, callback );
}

// MATCH-ONLY: the SDK's template CreateCallback, written out as this instance's explicit
// specialization so that its annotation names the one instance; the body is the template's.
// FUNCTION: STARFLEETOP 0x005fba10
template<>
tMemberCallback< tMetaClientChatCtlrC, const std::string& > CreateCallback( tMetaClientChatCtlrC* object, void (tMetaClientChatCtlrC::*func)( const std::string& result ) )
{
	return tMemberCallback< tMetaClientChatCtlrC, const std::string& >( object, func );
}

// MATCH-ONLY: the SDK's template destructor, written out as this instance's inline explicit
// specialization so that its annotation names the one instance. Its body (storing the base
// vtable) is the same as ~tCallback's, which the analysis names at this address.
// FUNCTION: STARFLEETOP 0x005fbb58
// FUNCTION: STARFLEETOP 0x005fba2f
template<>
inline tMemberCallback< tMetaClientChatCtlrC, const std::string& >::~tMemberCallback()
{}
// HYPOTHESIS: as for 0x005fb9d1, the first argument holds the first and past-the-end string at
// +0 and +4; it is written as a pair of pointers.
// FUNCTION: STARFLEETOP 0x005fba36
void FUN_005fba36( const std::pair<const std::string*, const std::string*>& strings, const tMemberCallback< tMetaClientChatCtlrC, const std::string& >& callback )
{
	std::for_each( strings.first, strings.second, callback );
}

// HYPOTHESIS: as for 0x005fba36, the first argument holds the first and past-the-end string at
// +0 and +4; it is written as a pair of pointers.
// FUNCTION: STARFLEETOP 0x005fba75
const std::string* FUN_005fba75( const std::pair<const std::string*, const std::string*>& strings, const std::string& value )
{
	return std::find( strings.first, strings.second, value );
}

#include "unsorted/fn_0055fe12_value.h"

// MATCH-ONLY: the library's _Construct for one element, written out so that it has source lines
// here; not inline, since nothing in this unit instantiates it.
namespace std
{
void _Construct( fn_0055fe12_value *_P, const fn_0055fe12_value& _V );
}

// FUNCTION: STARFLEETOP 0x005fba9d
void std::_Construct( fn_0055fe12_value *_P, const fn_0055fe12_value& _V )
{
	new ((void *)_P) fn_0055fe12_value( _V );
}

// MATCH-ONLY: the library's _Construct for the node value of the map of vectors (0x005fb06b),
// written out so that it has source lines here, as for 0x005fba9d.
// HYPOTHESIS: the pair is std::map<int, std::vector<int> >'s value; 0x0046180d is the vector's
// copy constructor.
namespace std
{
void _Construct( pair<const int, vector<int> > *_P, const pair<const int, vector<int> >& _V );
}

// FUNCTION: STARFLEETOP 0x005fbaca
void std::_Construct( std::pair<const int, std::vector<int> > *_P, const std::pair<const int, std::vector<int> >& _V )
{
	new ((void *)_P) std::pair<const int, std::vector<int> >( _V );
}

// MATCH-ONLY: the library's for_each, written out as the explicit specialization that
// 0x005fb9d1 calls so that its annotation names the one instance; the body is the template's.
// FUNCTION: STARFLEETOP 0x005fbb74
template<>
tMemberCallback< tMetaClientChatCtlrC, const tChatChannel& > std::for_each( const tChatChannel* _F, const tChatChannel* _L, tMemberCallback< tMetaClientChatCtlrC, const tChatChannel& > _Op )
{
	for (; _F != _L; ++_F)
		_Op(*_F);
	return (_Op);
}

// MATCH-ONLY: the library's for_each, written out as the explicit specialization that
// 0x005fba36 calls, as for 0x005fbb74.
// FUNCTION: STARFLEETOP 0x005fbbc1
template<>
tMemberCallback< tMetaClientChatCtlrC, const std::string& > std::for_each( const std::string* _F, const std::string* _L, tMemberCallback< tMetaClientChatCtlrC, const std::string& > _Op )
{
	for (; _F != _L; ++_F)
		_Op(*_F);
	return (_Op);
}

// HYPOTHESIS: a global byte string; its dynamic initializer and atexit registration.
// FUNCTION: STARFLEETOP 0x005fbe7d
// FUNCTION: STARFLEETOP 0x005fbe4a
std::string g_s00c39a70("MetaChatBar");

// HYPOTHESIS: a global byte string; its dynamic initializer and atexit registration.
// FUNCTION: STARFLEETOP 0x005fbee3
// FUNCTION: STARFLEETOP 0x005fbeb0
std::string g_s00c39a58("MetaChat");

// HYPOTHESIS: a global byte string; its dynamic initializer.
// FUNCTION: STARFLEETOP 0x005fbf16
std::string g_s00c39a48("BigAssChat");

// MATCH-ONLY: the caller of 0x005fb00a is not written yet; VC6 emits the specialization only
// once the class is instantiated. Drop this line when the caller lands.
template class std::map<int, int>;
template class std::map<int, std::vector<int> >;
template class std::vector<std::string>;
template class std::map<int, std::string>;
template class std::map<std::string, int>;
