#pragma once
#include "Arista.hpp"

Edge::Edge() {}

Edge::Edge( Vertex &&_vi, Vertex &&_vf ) : vi( std::move( _vi ) ), vf( std::move( _vf ) ) {}

bool Edge::operator==( const Edge &_arista )
{
	if( vi == _arista.vi && vf == _arista.vf )
		return( true );

	else if( vf == _arista.vi && vi == _arista.vf )
		return( true );

	return( false );
}

Vertex Edge::GetInitialVertex() { return( vi ); }

Vertex Edge::GetFinalVertex() { return( vf ); }

std::ostream & operator<<( std::ostream &_os, const Edge &_T )
{
	_os << "\tvi: " << _T.vi ;
	_os << "\t\t\tvf: " << _T.vf ;

	return( _os );
}

