#pragma once
#include "LectorArchivos.hpp"
#include <fstream>
#include <sstream>
#include <regex>

LectorArchivosOBJ::LectorArchivosOBJ() 
	:	verticesObjetoActual(),
		aristasObjetoActual(),
		carasObjetoActual(),
		objetosLeidos()
{
}

std::vector < Objeto > LectorArchivosOBJ::LeeObjetosArchivo( std::ifstream _archivo )
{
	std::string lineaTexto ;
	std::string nombre ;
	char c = '0' ;
	bool nuevoObjeto = false ;

	while( std::getline( _archivo, lineaTexto ) )
	{
		std::istringstream in( lineaTexto );
		
		if( in )
		{
			if( !(in >> c) )
				continue ;

			if( c == 'g' || c == 'o' || c == 'G' || c == 'O' )
			{
				if( nuevoObjeto )
				{
					aristasObjetoActual = DameAristasDeCaras( carasObjetoActual );

					Objeto objectoActual(	nombre, 
											std::move( carasObjetoActual ) );
					
					objectoActual.top = objectoActual.GetTopDistance();
					objetosLeidos.push_back( std::move( objectoActual ) );
					verticesObjetoActual.clear();
					aristasObjetoActual.clear();
					carasObjetoActual.clear();
				}	

				in >> nombre ;
				nuevoObjeto = true ;
			}
			
			else if( c == 'v' || c == 'V' )
			{
				c = in.get();
				if( c == 'n' || c == 't' )
					continue ;

				float x ;
				float y ;
				float z ;
				
				in >> x >> y >> z ;

				verticesObjetoActual.push_back( std::move( Vertex( x, y, z ) ) );
			}

			else if( c == 'f' || c == 'F' )
				carasObjetoActual.push_back( std::move( DameCaraDeCadena( lineaTexto ) ) );

		}
	}

	Objeto objectoActual(	nombre, 
							std::move( carasObjetoActual ));

	objectoActual.top = objectoActual.GetTopDistance();
	objetosLeidos.push_back( std::move( objectoActual ) );

	return( objetosLeidos );
}

Face LectorArchivosOBJ::DameCaraDeCadena( std::string _cadenaConCara )
{
	std::regex patron_1( "([0-9]+)[/]([0-9]*)[/]([0-9]*)" );	//Notación v1/vt1/vn1
	std::regex patron_2( "([0-9]+)" );	//Notación v1 v2 v3 v4...
	std::vector < Vertex > vertices ;
	std::vector < int > indiceVertices ;

	if( std::regex_search( _cadenaConCara, patron_1 ) )
	{
		std::sregex_iterator i ( _cadenaConCara.begin(), _cadenaConCara.end( ), patron_1 );
		for( ; i != std::sregex_iterator() ; ++i )
		{
			std::smatch matches = *i ;
			int indice = stoi( matches[1]  ) ;
			vertices.push_back( verticesObjetoActual[indice - 1] );
			indiceVertices.push_back( indice );
		}
	}

	else if( std::regex_search( _cadenaConCara, patron_2 ) )
	{
		std::sregex_iterator i ( _cadenaConCara.begin( ), _cadenaConCara.end( ), patron_2 );
		for( ; i != std::sregex_iterator() ; ++i )
		{
			std::smatch matches = *i ;
			int indice = stoi( matches[1]  ) ;
			vertices.push_back( verticesObjetoActual[indice - 1] );
			indiceVertices.push_back( indice );
		}
	}

	std::vector < Edge > aristas ;
	int tamVector = static_cast<int>( vertices.size() );
	for( int j = 1 ; j < tamVector ; ++j )
	{	
		Vertex vi = vertices[j-1] ;
		Vertex vf = vertices[j] ;
		aristas.push_back( std::move( Edge( std::move( vi ), std::move( vf ) ) ) );
	}

	aristas.push_back( std::move( Edge( std::move( vertices[vertices.size() - 1] ),
										std::move( vertices[0] ) ) ) );

	Face caraLeida( std::move( aristas ) );
	caraLeida.AgregaIndices( indiceVertices );

	return( caraLeida );
}

std::vector < Edge > LectorArchivosOBJ::DameAristasDeCaras( const std::vector < Face > &_caras )
{
	std::vector < Edge > aristas ;
	std::vector < Face > caras = _caras ;
	
	int tamCaras = static_cast<int>( _caras.size() );
	
	for( int i = 0 ; i < tamCaras ; ++i )
	{
		int tamAristas = static_cast<int>( caras[i].GetEdges().size() );

		for( int j = 0 ; j < tamAristas ; ++j )
			aristas.push_back( caras[i].GetEdges()[j] );
	}

	return( aristas );
}

