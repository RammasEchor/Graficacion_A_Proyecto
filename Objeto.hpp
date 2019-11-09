#pragma once
#include <string>
#include <vector>
#include "Cara.hpp"
#include <GLFW\glfw3.h>

class Objeto {
private:
	std::string nombre ;
	std::vector < Face > faces ;
	
public:
	float top ;
	Objeto( std::string _nombre, 
			std::vector < Face > &&_faces  );

	Objeto( const Objeto& _objeto );
	Objeto& operator=( const Objeto& _objeto ) = default ;
	const std::vector < Face >& GetFaces() const { return faces ; }
	Vertex GetFurthestVertex();
	float GetTopDistance();
	friend std::ostream & operator<<( std::ostream &_os, const Objeto &_T );
};