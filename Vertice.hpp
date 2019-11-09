#pragma once
#include <ostream>
#include <armadillo>

class Vertex {
private:
	arma::frowvec coord ;

public:
	Vertex();
	Vertex( float _x, float _y, float _z );

	//Constructor y asignación copia.
	Vertex( const Vertex & _vertex );
	Vertex& operator=( const Vertex &_vertice );

	//Constructor move
	Vertex( Vertex&& _vertex );

	bool operator==( const Vertex &_vertice );
	const arma::frowvec& get_coords() const { return( coord ); }
	void set_coords( const arma::fmat& _trans );
	arma::fcolvec homog_coords();
	float GetMag();
	friend std::ostream & operator<<( std::ostream &_os, const Vertex &_T );
};

