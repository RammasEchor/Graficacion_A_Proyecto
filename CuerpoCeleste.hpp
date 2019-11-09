#pragma once
#include "Objeto.hpp"
#include <armadillo>

class CuerpoCeleste
{
private:
	Objeto modelo ;
	float color[3] ;
	float distancia_centro ;
	float angulo_traslacion ;
	float angulo_rotacion ;
	float angulo_tilt ;

public:
	CuerpoCeleste( Objeto modelo, float _dist_cntr, 
					float _a_tras, float _a_rot, 
					float _a_tilt, float _color[] );
	void mueve();
	std::vector < Vertex > get_actual_vertex();
};
