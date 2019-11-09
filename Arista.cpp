#pragma once
#include "Arista.hpp"

Edge::Edge() {}

Edge::Edge( const Edge & _arista )
{
	vi = _arista.vi ;
	vf = _arista.vf ;
}

Edge::Edge( Edge&& _arista ) : vi( std::move( _arista.vi ) ), 
								vf( std::move( _arista.vf ) )
{
}

Edge::Edge( Vertex&& _vi, Vertex&& _vf ) : vi( _vi ), vf( _vf ) {}

Edge& Edge::operator=( const Edge& _arista )
{
	vi = _arista.vi ;
	vf = _arista.vf ;
	return( *this );
}

bool Edge::operator==( const Edge &_arista )
{
	if( vi == _arista.vi && vf == _arista.vf )
		return( true );

	else if( vf == _arista.vi && vi == _arista.vf )
		return( true );

	return( false );
}

std::ostream & operator<<( std::ostream &_os, const Edge &_T )
{
	_os << "\tvi: " << _T.vi ;
	_os << "\t\t\tvf: " << _T.vf ;

	return( _os );
}

