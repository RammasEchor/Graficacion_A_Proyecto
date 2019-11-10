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
	float vel_trasl_grad ;
	float angulo_rotacion ;
	float vel_rot_grad ;
	float angulo_tilt ;
	float escala ;

public:
	CuerpoCeleste( Objeto modelo, float _escala, float _dist_cntr, 
					float _a_tilt, float _vel_trasl_grados, 
						float _v_rot_grad, float _color[] );

	std::vector < arma::frowvec > get_actual_vertex();
	float* get_color() { return( color ); }
	void step();
};