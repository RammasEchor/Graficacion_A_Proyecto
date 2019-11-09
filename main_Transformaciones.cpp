#include <iostream>
#include "LectorArchivos.hpp"
#include <math.h>

int main()
{
	Vertex v1 = Vertex( 3, 4, 5 );
	Vertex v2 = Vertex( 6, 8, 9 );

	arma::frowvec p1 = v1.GetCoords();
	arma::frowvec p2 = v2.GetCoords();

	float D1 = sqrt( powf(p2[0] - p1[0], 2) + powf(p2[2] - p1[2], 2) );
	float D2 = sqrt( powf(p2[0] - p1[0], 2) + powf(p2[1] - p1[1], 2) + powf(p2[2] - p1[2], 2) );

	arma::fmat T = { 
					{ 1, 0, 0, -p1[0] },
					{ 0, 1, 0, -p1[1] },
					{ 0, 0, 1, -p1[2] },
					{ 0, 0, 0, 1 }
										};

	arma::fmat Ry = {
						{ ( p2[0] - p1[0] )/D1, 0, ( p2[2] - p1[2] )/D1, 0 },
						{ 0, 1, 0, 0 },
						{ -( p2[2] - p1[2] )/D1, 0, ( p2[0] - p1[0] )/D1, 0},
						{ 0, 0, 0, 1 }
																				};
	arma::fmat Rz = {
						{ D1/D2, (p2[1] - p1[1])/D2, 0, 0 },
						{ -(p2[1] - p1[1])/D2, D1/D2, 0, 0 },
						{  0, 0, 1, 0 },
						{ 0, 0, 0, 1 }
																				};

	arma::fcolvec v1pp = Rz * Ry * T * v1.CoordHomog();
	arma::fcolvec v2pp = Rz * Ry * T * v2.CoordHomog();

	v1pp.print();
	v2pp.print();
}