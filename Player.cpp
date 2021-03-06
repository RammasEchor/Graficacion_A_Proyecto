#pragma once

#include "Player.hpp"
#include "Transform.hpp"

#define MAX_X 2
#define MAX_Y 2
#define PLAYER_Z 10 

Player::Player( Objeto& _modelo )
{
	modelo = &_modelo ;
	posicion_curva = 0.0f ;
	Transform t ;
	mat_transf_inicial = t.T( 0.0f, 0.0f, 1.0f );
	mat_transf_actual = t.T( 0.0f, 0.0f, 1.0f ) * t.S( 0.1f, 0.1f, 0.1f );

	hermite_mat = {	{ 2.0f, -2.0f, 1.0f, 1.0f  },
					{ -3.0f, 3.0f, -2.0f, -1.0f },
					{ 0.0f, 0.0f, 1.0f, 0.0f },
					{ 1.0f, 0.0f, 0.0f, 0.0f }		};

	arma::frowvec P1 { 0.0f, 0.0f };
	arma::frowvec R1 { 0.5f, 0.0f };
	arma::frowvec P4 { 1.0f, 0.1f };
	arma::frowvec R4 { 0.2f, 0.2f };

	puntos_hermite.push_back( std::move( P1 ) );
	puntos_hermite.push_back( std::move( R1 ) );
	puntos_hermite.push_back( std::move( P4 ) );
	puntos_hermite.push_back( std::move( R4 ) );
}

void Player::ladea( int _movimiento )
{
	posicion_curva += ( _movimiento * 0.1f );
	posicion_curva = posicion_curva > 1.0f ? 0.999f : 
						posicion_curva < -1.0f ? -0.999f : posicion_curva ;

	float temp_pos = abs( posicion_curva );

	arma::frowvec T { temp_pos * temp_pos * temp_pos,
						temp_pos * temp_pos,
						temp_pos, 1 };

	arma::fmat TmH = T * hermite_mat ;

	arma::fmat x_t = TmH * arma::fcolvec{ { puntos_hermite[0][0] },
											{ puntos_hermite[2][0] },
											{ puntos_hermite[1][0] },
											{ puntos_hermite[3][0] } } ;

	arma::fmat y_t = TmH * arma::fcolvec{ { puntos_hermite[0][1] },
											{ puntos_hermite[2][1] },
											{ puntos_hermite[1][1] },
											{ puntos_hermite[3][1] } };

	float signo = posicion_curva >= 0.0f ? 1.0f : -1.0f ;
	Transform t ;
	mat_transf_actual = mat_transf_inicial * t.T( signo * x_t[0], 0.0f, 0.0f );
	mat_transf_actual = mat_transf_actual * t.T( 0.0f, y_t[0], 0.0f ) * t.S( 0.1f, 0.1f, 0.1f );
}

std::vector < arma::frowvec > Player::dame_posicion()
{
	std::vector < arma::frowvec > posicion_actual ;
	arma::fcolvec v_col ;
	for( arma::frowvec v : modelo->get_vertex() )
	{
		v_col = { { v[0] }, { v[1] }, { v[2] }, 1 };
		v_col = mat_transf_actual * v_col ;
		posicion_actual.push_back( std::move( arma::frowvec( { v_col[0], v_col[1], v_col[2] } ) ) );
	}

	return( posicion_actual );
}

std::vector < Face > Player::dame_caras_transf()
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
			v_col = mat_transf_actual * v_col ;
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