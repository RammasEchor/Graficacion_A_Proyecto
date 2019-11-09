#pragma once
#include <vector>
class Vertex {
private:
	double x, y, z ;
public:
	Vertex( double x_, double y_, double z_ ) : x( x_ ), y( y_ ), z( z_ ) {}
	Vertex() : x( 0 ), y( 0 ), z( 0 ) {}
};

class Edge {
private:
	Vertex vi, vf ;
public:
	Edge( Vertex vi_, Vertex vf_ ) : vi( vi_ ), vf( vf_ ) {}
};

class Face {
private:
	std::vector<Edge> edges ;
public:
	Face() : {}
};