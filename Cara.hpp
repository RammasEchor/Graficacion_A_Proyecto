#pragma once
#include <vector>
#include <ostream>
#include "Arista.hpp"

class Face {
private:
	std::vector < Edge > edges ;
	std::vector < int > indiceVertices ;
	arma::frowvec normalNormalizada ;
public:
	Face();
	Face( const Face & );
	Face( Face&& _face );
	Face( std::vector < Edge > &&_edges );
	Face& operator=( const Face& _cara );
	void AgregaArista( Edge&& _e );
	const std::vector < Edge >& GetEdges() const { return( edges ); }
	const std::vector < int >& GetIndexes() const { return( indiceVertices ); }
	std::vector < Vertex > GetTriangle() const ;
	void CalculaNormal();
	arma::frowvec dame_normal_normalizada() const ;
	void AgregaIndices( std::vector < int > _indices );
	friend std::ostream & operator<<( std::ostream &_os, const Face &_T );
};