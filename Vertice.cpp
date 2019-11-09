#pragma once
#include "Vertice.hpp"

Vertex::Vertex() : coord( { 0, 0, 0 } ) {}

//Constructor de traslado
Vertex::Vertex( Vertex &&_vertex ) : coord( std::move( _vertex.coord ) ) {}

Vertex::Vertex( float _x, float _y, float _z ) : coord( { _x, _y, _z } ) {}

bool Vertex::operator == ( const Vertex &_vertice )
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

void Vertex::operator = ( const Vertex &_vertice )
{
	coord = { _vertice.coord[0], _vertice.coord[1], _vertice.coord[2] };
}

arma::frowvec Vertex::GetCoords() { return( coord ); }

arma::fcolvec Vertex::CoordHomog()
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

void Vertex::set_value( arma::fmat _trans )
{
	coord[0] = _trans[0] ;
	coord[1] = _trans[1] ;
	coord[2] = _trans[2] ;
}

arma::frowvec Vertex::get_value()
{
	return( coord );
}