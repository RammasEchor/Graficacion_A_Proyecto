#pragma once
#include "Objeto.hpp"
#include "CuerpoCeleste.hpp"
#include "Transform.hpp"


CuerpoCeleste::CuerpoCeleste( Objeto _modelo, float _escala, float _dist_cntr, 
								float _a_tilt, float _v_trasl_grados, 
									float _v_rot_grad, float _color[] )
{
	modelo = _modelo ;
	distancia_centro = _dist_cntr ;
	angulo_traslacion = 0.0f ;
	angulo_rotacion = 0.0f ;
	angulo_tilt = _a_tilt ;
	escala = _escala ;
	vel_trasl_grad = _v_trasl_grados ;
	vel_rot_grad = _v_rot_grad ;

	for( int i = 0 ; i < 3 ; ++i )
		color[i] = _color[i] ;
}

std::vector < arma::frowvec > CuerpoCeleste::get_actual_vertex()
{
	Transform t ;
	arma::fmat transf = t.S( escala, escala, escala );
	transf = transf * t.R( 0.0f, 1.0f, 0.0f, angulo_traslacion ) * 
						t.T( distancia_centro, 0.0f, 0.0f ) *
						t.R( 0.0f, 0.0f, 1.0f, angulo_tilt ) *
						t.R( 0.0f, 1.0f, 0.0f, angulo_rotacion );

	std::vector < arma::frowvec > vertices_transformados ;
	for( Face cara : modelo.GetFaces() )
	{
		std::vector< Vertex > triangle_vertex = cara.GetTriangle();
		for( int i = 0 ; i < 3 ; ++i )
		{
			const arma::frowvec vertex = triangle_vertex[i].get_coords();
			arma::fcolvec v = { { vertex[0] }, { vertex[1] }, { vertex[2] }, 1 };
			v = transf * v ;

			vertices_transformados.push_back( arma::frowvec { v[0], v[1], v[2] } );
		}
	}

	return( std::move( vertices_transformados ) );
}

void CuerpoCeleste::step()
{
	angulo_traslacion = angulo_traslacion < 360.0f ? angulo_traslacion + vel_trasl_grad : 0 ; 
	angulo_rotacion = angulo_rotacion < 360.0f ? angulo_rotacion + vel_rot_grad : 0 ;
}