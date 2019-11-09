#pragma once
#include <ostream>
#include "Vertice.hpp"

class Edge {
private:
	Vertex vi, vf ;

public:
	Edge();
	Edge( const Edge & );
	Edge( Edge&& _arista );
	Edge( Vertex&& _vi, Vertex&& _vf );
	Edge& operator=( const Edge& _arista );
	bool operator==( const Edge &_arista );
	const Vertex& GetInitialVertex() const { return( vi ); }
	const Vertex& GetFinalVertex() const { return( vf ); }
	friend std::ostream & operator<<( std::ostream &_os, const Edge &_T );
};