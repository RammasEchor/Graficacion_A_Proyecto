#pragma once
#include <armadillo>
#include "Objeto.hpp"

class Obstaculo	{
	private:
		arma::fmat mat_transf ;
		Objeto* modelo ;

	public:
		Obstaculo( Objeto &_modelo );
		void establece_posicion_inicial( arma::fmat &&_mat_ini );
		void avanza( const arma::fmat &_dist );
		std::vector < arma::frowvec > dame_posicion();
};
