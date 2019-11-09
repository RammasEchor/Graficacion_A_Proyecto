#pragma once
#include "Cara.hpp"
#include <iterator>

Face::Face() : edges() {}

Face::Face( Face &&_face ) : edges( std::move( _face.edges ) ), indiceVertices( std::move( _face.indiceVertices ) ) {}

Face::Face( std::vector < Edge > &&_edges ) : edges( std::move( _edges ) ) {}

void Face::AgregaArista( Edge _e )
{
	edges.push_back( _e );
}

const std::vector < Edge >& Face::GetEdges()	{ return( edges ); }

const std::vector < int >& Face::GetIndexes()	{ return( indiceVertices ); }

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

arma::frowvec Face::GetNormal()
{
	Vertex one = edges[0].GetInitialVertex();
	Vertex two = edges[1].GetInitialVertex();
	Vertex three = edges[2].GetInitialVertex();

	arma::frowvec normal = arma::cross( two.GetCoords() - one.GetCoords(), 
										three.GetCoords() - one.GetCoords() );

	return( normal );
}

std::ostream & operator<<( std::ostream &_os, const Face &_T )
{
	for( int i = 0 ; i < 3 ; ++i )
		_os << _T.indiceVertices[i] << "\t" ;

	return( _os );
}