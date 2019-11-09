#pragma once
#include <ostream>
#include "Vertice.hpp"

class Edge {
private:
	Vertex vi, vf ;

public:
	Edge();
	Edge( const Edge & ) = default ;
	Edge( Vertex &&_vi, Vertex &&_vf );
	bool operator == ( const Edge &_arista );
	Vertex GetInitialVertex();
	Vertex GetFinalVertex();
	friend std::ostream & operator<<( std::ostream &_os, const Edge &_T );
};