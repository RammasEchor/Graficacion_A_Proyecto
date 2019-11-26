#pragma once

#include "LectorArchivosOBJ.hpp"

//Prototipos
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );
float Magnitud( const arma::frowvec& _vector );
float ProductoPunto( const arma::frowvec& _v1, const arma::frowvec& _v2 );
std::vector < Face > DameCarasVisibles( const arma::frowvec& _camera, const Objeto& _objeto );

int main()
{
	std::string nombreArchivo = "cube.obj" ;
	std::vector < Objeto > objetosLeidos = LeeObjetos( nombreArchivo );

	arma::frowvec eye { 40.0f, -31.0f, -24.0f };
	arma::frowvec camera { -5.0f, 0.0f, 3.0f };

	//Se trasladan de manera que el origen quede sobre ( 0, 0, 0 ).
	camera = camera - eye ;
	eye = eye - eye ;

	//Se normaliza el vector que representa hacia adonde apunta la camara.
	float cam_mag = Magnitud( camera );
	camera = camera / cam_mag ;

	Objeto& cubo = objetosLeidos[0];

	std::vector < Face > carasVisibles = DameCarasVisibles( camera, cubo );
	
}

std::vector < Objeto > LeeObjetos( std::string _nombreArchivo )
{
	clock_t cronometro = clock();

	LectorArchivosOBJ lector ;
	std::vector < Objeto > objetos = lector.LeeObjetosArchivo( std::ifstream( _nombreArchivo ) );

	printf("Lectura de objetos: %.2fs\n", (double)(clock() - cronometro)/CLOCKS_PER_SEC);

	return( objetos );
}

float Magnitud( const arma::frowvec& _vector )
{
	float magnitud = sqrtf( powf( _vector[0], 2 ) + powf( _vector[1], 2 ) + powf( _vector[2], 2 ) );
	return( magnitud );
}

float ProductoPunto( const arma::frowvec& _v1, const arma::frowvec& _v2 )
{
	float x_r = _v1[0] * _v2[0] ;
	float y_r = _v1[1] * _v2[1] ;
	float z_r = _v1[2] * _v2[2] ;

	return( x_r + y_r + z_r );
}

std::vector < Face > DameCarasVisibles( const arma::frowvec& _camera, const Objeto& _objeto )
{
	std::vector < Face > normalesVisibles ;
	for( const Face f : _objeto.GetFaces() )
	{
		arma::frowvec normalnorm = f.dame_normal_normalizada();
		float cos_theta = ProductoPunto( normalnorm, _camera );

		if( cos_theta < 0.0f )
			normalesVisibles.push_back( f );
	}

	return( normalesVisibles );
}