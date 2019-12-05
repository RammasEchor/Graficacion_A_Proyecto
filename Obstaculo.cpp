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

std::vector < Face > Obstaculo::dame_caras_transf()
{
	std::vector < Face > caras_transf ;
	
	for( auto f : modelo->GetFaces() )
	{
		std::vector < Vertex > vert_cara = f.GetTriangle();
		std::vector < arma::frowvec > vert_transf ;
		arma::fcolvec v_col ;
		for( Vertex v : vert_cara )
		{
			v_col = { { v.get_coords()[0] }, { v.get_coords()[1] }, { v.get_coords()[2] }, 1 };
			v_col = mat_transf * v_col ;
			vert_transf.push_back( arma::frowvec( { v_col[0], v_col[1], v_col[2] } ) );
		}

		Vertex v1_1( vert_transf[0][0], vert_transf[0][1], vert_transf[0][2] );
		Vertex v1_2( vert_transf[0][0], vert_transf[0][1], vert_transf[0][2] );
		Vertex v2_1( vert_transf[1][0], vert_transf[1][1], vert_transf[1][2] );
		Vertex v2_2( vert_transf[1][0], vert_transf[1][1], vert_transf[1][2] );
		Vertex v3_1( vert_transf[2][0], vert_transf[2][1], vert_transf[2][2] );
		Vertex v3_2( vert_transf[2][0], vert_transf[2][1], vert_transf[2][2] );

		std::vector < Edge > aristas ;
		aristas.push_back( Edge( std::move( v1_1 ), std::move( v2_1 ) ) );
		aristas.push_back( Edge( std::move( v2_2 ), std::move( v3_1 ) ) );
		aristas.push_back( Edge( std::move( v3_2 ), std::move( v1_2 ) ) );

		Face f( std::move( aristas ) );
		caras_transf.push_back( std::move( f ) );
	}

	return( caras_transf );
}