#pragma once
#include "Objeto.hpp"
#include "CuerpoCeleste.hpp"


CuerpoCeleste::CuerpoCeleste( Objeto _modelo, float _dist_cntr, 
								float _a_tras, float _a_rot, 
								float _a_tilt, float _color[] )
{
	modelo = _modelo ;
	distancia_centro = _dist_cntr ;
	angulo_traslacion = _a_tras ;
	angulo_rotacion = _a_rot ;
	angulo_tilt = _a_tilt ;

	for( int i = 0 ; i < 3 ; ++i )
		color[i] = _color[i] ;
}

void CuerpoCeleste::mueve()
{
	
}