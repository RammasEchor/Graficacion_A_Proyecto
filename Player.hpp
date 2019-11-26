#pragma once
#include "Objeto.hpp"

class Player
{
	private:
		Objeto* modelo ;
		float posicion_curva ;
		arma::fmat mat_transf_actual ;
		arma::fmat mat_transf_inicial ;
		arma::fmat hermite_mat ;
		std::vector < arma::frowvec > puntos_hermite ; //P1, R1, P4, R4.

	public:
		Player( Objeto& _modelo );
		void ladea( int _direccion );
		std::vector< arma::frowvec > dame_posicion();
};