#pragma once
#include <vector>
#include <ostream>
#include "Arista.hpp"

class Face {
private:
	std::vector < Edge > edges ;
	std::vector < int > indiceVertices ;
public:
	Face();
	Face( const Face & );
	Face( Face&& _face );
	Face( std::vector < Edge > &&_edges );
	Face& operator=( const Face& _cara );
	void AgregaArista( Edge&& _e );
	const std::vector < Edge >& GetEdges() const { return( edges ); }
	const std::vector < int >& GetIndexes() const { return( indiceVertices ); }
	std::vector < Vertex > GetTriangle();
	arma::frowvec GetNormal();
	void AgregaIndices( std::vector < int > _indices );
	friend std::ostream & operator<<( std::ostream &_os, const Face &_T );
};