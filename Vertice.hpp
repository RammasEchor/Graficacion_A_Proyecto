#pragma once
#include <ostream>
#include <armadillo>

class Vertex {
private:
	arma::frowvec coord ;

public:
	Vertex();
	Vertex( const Vertex & ) = default ;
	Vertex( Vertex &&_vertex );
	Vertex( float _x, float _y, float _z );
	void operator = ( const Vertex &_vertice );
	bool operator == ( const Vertex &_vertice );
	arma::frowvec GetCoords();
	arma::frowvec get_value();
	arma::fcolvec CoordHomog();
	float GetMag();
	void set_value( arma::fmat _trans );
	friend std::ostream & operator<<( std::ostream &_os, const Vertex &_T );
};

