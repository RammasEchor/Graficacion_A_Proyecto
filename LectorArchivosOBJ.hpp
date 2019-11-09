#pragma once
#include <vector>
#include <string>
#include <ostream>
#include "Objeto.hpp"

class LectorArchivosOBJ
{
private:
	std::vector < Vertex > verticesObjetoActual ;
	std::vector < Edge > aristasObjetoActual ;
	std::vector < Face > carasObjetoActual ;
	std::vector < Objeto > objetosLeidos ;

	Face DameCaraDeCadena( std::string _cadenaConCara );
	std::vector < Edge > DameAristasDeCaras( const std::vector < Face >& _caras );

public:
	LectorArchivosOBJ();
	std::vector < Objeto > LeeObjetosArchivo( std::ifstream _archivo );
};

