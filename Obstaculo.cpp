#pragma once
#include "Obstaculo.hpp"
#include "Transform.hpp"

Obstaculo::Obstaculo( Objeto &_modelo )
{
	modelo = &_modelo;
}

void Obstaculo::establece_posicion_inicial( arma::fmat &&_mat_init )
{
	mat_transf = _mat_init ;
}

void Obstaculo::avanza( const arma::fmat &_dist )
{
	mat_transf = mat_transf * _dist ;
}

std::vector < arma::frowvec > Obstaculo::dame_posicion()
{
	std::vector < arma::frowvec > posicion_actual ;
	arma::fcolvec v_col ;
	for( arma::frowvec v : modelo->get_vertex() )
	{
		v_col = { { v[0] }, { v[1] }, { v[2] }, 1 };
		v_col = mat_transf * v_col ;
		posicion_actual.push_back( std::move( arma::frowvec( { v_col[0], v_col[1], v_col[2] } ) ) );
	}

	return( posicion_actual );
}