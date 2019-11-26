#pragma once

#include <stdlib.h>
#include <time.h>
#include "FabricaObstaculos.hpp"
#include "Transform.hpp"

#define SPAN_OBSTACLE_WIDTH 20
#define OFFSET_RAND (float)SPAN_OBSTACLE_WIDTH / 2 

FabricaObstaculos::FabricaObstaculos( Objeto &_modelo )
{
	modelo = &_modelo ;

	Transform t ;
	srand( (unsigned int)time( NULL ) );

	arma::fmat initial_mat = t.S( 0.15f, 0.15f, 0.15f );
	for( int i = 0 ; i < 9 ; ++i )
	{
		Obstaculo obs( *modelo );
		arma::fmat initial_obs_mat = initial_mat * t.T( rand() % SPAN_OBSTACLE_WIDTH - OFFSET_RAND, 0.0f, 0.0f - (i * 3) );
		obs.establece_posicion_inicial( std::move( initial_obs_mat ) );
		obstaculos.push_back( std::move( obs ) );
	}
}

const std::vector < Obstaculo > FabricaObstaculos::dame_obstaculos()
{
	return( obstaculos );
}

void FabricaObstaculos::avanza( arma::fmat &_dist )
{
	Transform t ;
	size_t size = obstaculos.size();
	for( unsigned int i = 0 ; i < size ; ++i )
		obstaculos[i].avanza( _dist );

	std::vector < arma::frowvec > mas_avanzado = obstaculos[0].dame_posicion();
	if( mas_avanzado[0][2] > 2.0f )
	{
		obstaculos.erase( obstaculos.begin() );
		Obstaculo obs( *modelo );
		arma::fmat initial_obs_mat = t.S( 0.15f, 0.15f, 0.15f ) * t.T( rand() % SPAN_OBSTACLE_WIDTH - OFFSET_RAND, 0.0f, -15.0f );
		obs.establece_posicion_inicial( std::move( initial_obs_mat ) );
		obstaculos.push_back( std::move( obs ) );
	}
}