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

	Vertex DameVerticeDeCadena( std::string _cadenaConVertice );
	Face DameCaraDeCadena( std::string _cadenaConCara );
	std::vector < Edge > DameAristasDeCaras( std::vector < Face > _caras );
	std::vector < Edge > QuitaAristasRepetidas( std::vector < Edge > _aristas );

public:
	LectorArchivosOBJ();
	std::vector < Objeto > LeeObjetosArchivo( std::ifstream _archivo );
};



