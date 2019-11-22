#pragma once
#include "Obstaculo.hpp"

class FabricaObstaculos
{
	private:
		std::vector < Obstaculo > obstaculos ;
		Objeto* modelo ;

	public:
		FabricaObstaculos( Objeto &_modelo );
		const std::vector < Obstaculo > dame_obstaculos();
		void avanza( arma::fmat &_dist );
};
