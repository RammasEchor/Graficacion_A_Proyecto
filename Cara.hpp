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
	Face( const Face & ) = default ;
	Face( Face &&_face );
	Face( std::vector < Edge > &&_edges );
	void AgregaArista( Edge _e );
	const std::vector < Edge >& GetEdges();
	std::vector < Vertex > GetTriangle();
	const std::vector < int >& GetIndexes();
	arma::frowvec GetNormal();
	void AgregaIndices( std::vector < int > _indices );
	friend std::ostream & operator<<( std::ostream &_os, const Face &_T );
};