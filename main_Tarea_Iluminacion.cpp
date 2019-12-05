#pragma once

#include "LectorArchivosOBJ.hpp"

//Prototipos
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );
float Magnitud( const arma::frowvec& _vector );
float ProductoPunto( const arma::frowvec& _v1, const arma::frowvec& _v2 );
std::vector < Face > DameCarasVisibles( const arma::frowvec& _camera, const Objeto& _objeto );
float DameRazonIluminado( const Face& _cara, const arma::frowvec& _foco );

int main()
{
	std::string nombreArchivo = "cube.obj" ;
	std::vector < Objeto > objetosLeidos = LeeObjetos( nombreArchivo );

	arma::frowvec eye { 40.0f, -31.0f, -24.0f };
	arma::frowvec camera { -5.0f, 0.0f, 3.0f };
	arma::frowvec foco { 30.0f, -24.0f, -24.0f };

	//Se trasladan de manera que el origen quede sobre ( 0, 0, 0 ).
	camera = camera - eye ;
	eye = eye - eye ;

	//Se normalizan el vector que representa la camara y el foco.
	float cam_mag = Magnitud( camera );
	camera = camera / cam_mag ;
	float foco_mag = Magnitud( foco );
	foco = foco / foco_mag ;

	Objeto& cubo = objetosLeidos[0];
	std::vector < Face > carasVisibles = DameCarasVisibles( camera, cubo );

	std::vector < std::tuple < Face, arma::frowvec > > colorCara ;
	arma::frowvec color { 211.0f, 187.0f, 24.0f };
	for( Face f : carasVisibles )
	{
		float razon_color = DameRazonIluminado( f, foco );
		arma::frowvec color_iluminado = color * razon_color ;
		colorCara.push_back( std::make_tuple( f, color_iluminado ) );
	}

	for( auto cara : colorCara )
	{
		std::vector < Vertex > triang = std::get<0>( cara ).GetTriangle();
		std::cout << "La cara con los vertices: " << std::endl ;
		for( Vertex v : triang )
		{
			const arma::frowvec coords = v.get_coords();
			std::cout << "x: " << coords[0] << "\t" ;
			std::cout << "y: " << coords[1] << "\t" ;
			std::cout << "z: " << coords[2] << std::endl ;
		}

		std::cout << "Es visible y tiene color de:" << std::endl ;
		arma::frowvec color = std::get<1>( cara );
		std::cout << color[0] << ", " << color[1] << ", " << color[2] << std::endl << std::endl ;
	}
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

float DameRazonIluminado( const Face& _cara, const arma::frowvec& _foco )
{
	float Ia = 0.95f ;
	float Ka = 0.58f ;
	float Ip = 0.8f ;
	float Kd = 0.75f ;

	float cos_theta = ProductoPunto( _cara.dame_normal_normalizada(), _foco );
	float I = ( Ia * Ka ) + ( Ip * Kd * cos_theta );

	return( I / 2 );
}