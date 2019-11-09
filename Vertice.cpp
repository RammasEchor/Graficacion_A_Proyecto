#pragma once
#include "Vertice.hpp"

Vertex::Vertex() : coord( { 0, 0, 0 } ) {}

Vertex::Vertex( const Vertex & _vertice )
{
	coord = _vertice.coord ;
}

//Constructor de traslado
Vertex::Vertex( Vertex&& _vertice ) : coord( std::move( _vertice.coord ) )
{
}

Vertex::Vertex( float _x, float _y, float _z ) : coord( { _x, _y, _z } ) {}

Vertex& Vertex::operator=( const Vertex &_vertice )
{
	coord = _vertice.coord ;
	return *this ;
}

bool Vertex::operator==( const Vertex &_vertice )
{
	if( coord[0] == _vertice.coord[0] )
		if( coord[1] == _vertice.coord[1] )
			if( coord[2] == _vertice.coord[2] )
				return( true );

	return( false );
}

float Vertex::GetMag()
{
	return( _CMATH_::sqrt( _CMATH_::pow( coord[0], 2 ) + _CMATH_::pow( coord[1], 2 ) + _CMATH_::pow( coord[2], 2 ) ) );
}

arma::fcolvec Vertex::homog_coords()
{
	arma::fcolvec p = { {coord[0]}, {coord[1]}, {coord[2]}, {1} };
	return( p ); 
}

std::ostream & operator<<( std::ostream& _os, const Vertex& _T )
{
	_os << std::fixed ;
	_os.precision( 4 );
	_os << "\t" << _T.coord[0] << "\t\t" << _T.coord[1] << "\t\t" << _T.coord[2] << std::endl ;
	return( _os );
}

void Vertex::set_coords( const arma::fmat& _trans )
{
	coord[0] = _trans[0] ;
	coord[1] = _trans[1] ;
	coord[2] = _trans[2] ;
}