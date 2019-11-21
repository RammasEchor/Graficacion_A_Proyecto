#pragma once
#include "Objeto.hpp"
#include <iterator>

Objeto::Objeto()
{
}

Objeto::Objeto( const Objeto& _objeto )
{
	faces = _objeto.faces ;
	nombre = _objeto.nombre ;
}

Objeto::Objeto( std::string _nombre, 
				std::vector < Face > &&_faces )
	: nombre( _nombre ),  
	  faces( std::move( _faces ) )
{
}

Vertex Objeto::GetFurthestVertex()
{
	Vertex vertex( { 0, 0, 0 } );

	for( Face cara : faces )
	{
		for( Edge arista : cara.GetEdges() )
		{
			Vertex vi = arista.GetInitialVertex();
			Vertex vf = arista.GetFinalVertex();
			if( !(vertex.GetMag() > vi.GetMag() ) )
				vertex = arista.GetInitialVertex();

			if( !(vertex.GetMag() > vf.GetMag() ) )
				vertex = arista.GetFinalVertex();
		}
	}

	return( vertex );
}

float Objeto::GetTopDistance()
{
	Vertex v = GetFurthestVertex();

	return( v.GetMag() );
}

std::vector < arma::frowvec > Objeto::get_vertex()
{
	std::vector< arma::frowvec > vertex ;
	for( Face cara : faces )
	{
		std::vector< Vertex > triangle_vertex = cara.GetTriangle();
		for( int i = 0 ; i < 3 ; ++i )
		{
				vertex.push_back(	
					arma::frowvec { 
						triangle_vertex[i].get_coords()[0], 
						triangle_vertex[i].get_coords()[1],
						triangle_vertex[i].get_coords()[2] } );
		}
	}

	return( std::move( vertex ) );
}

std::ostream& operator<<( std::ostream &_os, const Objeto &_T )
{
	/*_os << "Nombre: " << "\"" << _T.nombre << "\"" << std::endl ;

	int i = 1 ;
	std::vector < Vertex > vertices( 11 );
	for( Face cara : _T.GetFaces() )
	{
		int j = 0 ;
		for( Edge arista : cara.GetEdges() )
		{
			vertices[cara.GetIndexes()[j] - 1] = arista.GetInitialVertex();
			++j ;
		}
	}

	i = 1 ;
	for( Vertex v : vertices )
	{
		_os << "v" << i << "\t" << v ;
		++i ;
	}

	i = 1 ;
	for( Face cara : _T.GetFaces() )
	{
		for( Edge arista : cara.GetEdges() )
		{
			_os << "\tArista #" << i << ":" << arista << std::endl ;
			++i ;
		}
	}

	i = 1 ;
	for( Face cara : _T.GetFaces() )
	{
		_os << "\tFace #" << i << ": " << cara << std::endl ;
		++i ;
	}
	*/
	return( _os );
}

