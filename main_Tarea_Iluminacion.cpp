#pragma once

#include "LectorArchivosOBJ.hpp"

//Prototipos
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );

int main()
{
	std::string nombreArchivo = "cube.obj" ;
	std::vector < Objeto > objetosLeidos = LeeObjetos( nombreArchivo );

	Objeto& cubo = objetosLeidos[0];

	for( const Face f : cubo.GetFaces() )
	{
		arma::frowvec normalnorm = f.dame_normal_normalizada();
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