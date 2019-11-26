#pragma once
#include "Cara.hpp"
#include <iterator>

Face::Face() : edges() {}

Face::Face( const Face& _face )
{
	edges = _face.edges ;
}

Face::Face( Face &&_face ) : edges( std::move( _face.edges ) ), indiceVertices( std::move( _face.indiceVertices ) ) {}

Face::Face( std::vector < Edge > &&_edges ) : edges( std::move( _edges ) ) {}

Face& Face::operator=( const Face& _cara )
{
	edges = _cara.edges ;
	indiceVertices = _cara.indiceVertices ;
	return( *this );
}

void Face::AgregaArista( Edge&& _e )
{
	edges.push_back( _e );
}

std::vector < Vertex > Face::GetTriangle()
{
	std::vector < Vertex > vertex ;
	vertex.push_back( edges[0].GetInitialVertex() );
	vertex.push_back( edges[1].GetInitialVertex() );
	vertex.push_back( edges[2].GetInitialVertex() );
	return( vertex );
}

void Face::AgregaIndices( std::vector < int > _indices )
{
	indiceVertices.push_back( _indices[0] );
	indiceVertices.push_back( _indices[1] );
	indiceVertices.push_back( _indices[2] );
}

arma::frowvec Face::dame_normal_normalizada() const
{
	const Vertex& one = edges[0].GetInitialVertex();
	const Vertex& two = edges[1].GetInitialVertex();
	const Vertex& three = edges[2].GetInitialVertex();

	arma::frowvec normal = arma::cross( two.get_coords() - one.get_coords(), 
										three.get_coords() - one.get_coords() );

	float magnitud = sqrtf( powf( normal[0], 2 ) + powf( normal[1], 2 ) + powf( normal[2], 2 ) );

	for( int i = 0 ; i < 3 ; ++i )
		normal[i] = normal[i] / magnitud ;

	return( normal );
}

std::ostream & operator<<( std::ostream &_os, const Face &_T )
{
	for( int i = 0 ; i < 3 ; ++i )
		_os << _T.indiceVertices[i] << "\t" ;

	return( _os );
}