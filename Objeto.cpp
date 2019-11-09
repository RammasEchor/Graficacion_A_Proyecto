#pragma once
#include "Objeto.hpp"
#include <iterator>

Objeto::Objeto( std::string _nombre, 
				std::vector < Face > &&_faces )
	: nombre( _nombre ),  
	  faces( std::move( _faces ) )
{
}

const std::vector < Face >& Objeto::GetFaces() const { return( faces ); }

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

std::ostream & operator<<( std::ostream &_os, const Objeto &_T )
{
	_os << "Nombre: " << "\"" << _T.nombre << "\"" << std::endl ;

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

	return( _os );
}

std::vector< Vertex > Objeto::get_faces_verts()
{
	std::vector< Vertex > vertices ;
	for( Face cara : faces )
	{
		for( Edge arista : cara.GetEdges() )
		{
			vertices.push_back( arista.GetInitialVertex() );
			vertices.push_back( arista.GetFinalVertex() );
		}
	}

	return( vertices );
}

