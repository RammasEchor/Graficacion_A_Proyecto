//Lee y muestra los objetos leidos.
//
#pragma once
#include <iterator>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "LectorArchivosOBJ.hpp"
#include "Transform.hpp"
#include "CuerpoCeleste.hpp"

//Prototipos------------------------------------------
GLFWwindow* InicializaGLFW();
void InicializarCamaraGLFW( GLFWwindow* _window );
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );
std::vector < arma::frowvec > TransformaObjeto( Objeto& _objeto, arma::fmat _trans );
void DibujaObjeto( std::vector < arma::frowvec >& _vertices, float _color[] );
arma::fmat TransformaPlaneta( arma::fmat _matriz, float _dist_sol, 
								float _grados_trasl, float _grados_tilt, 
								float _grados_rot );
void DibujaCuerpoCeleste( CuerpoCeleste& _c );
//----------------------------------------------------

int main()
{
	std::string nombreArchivo = "roca.obj" ;
	std::vector < Objeto > objetos = LeeObjetos( nombreArchivo );

	GLFWwindow* window ;
	if( ( window = InicializaGLFW()) == nullptr )
		return( -1 );

	//Colores
	float amarillo[] = { 1.0f, 1.0f, 0.0f };
	float rojo[] = { 1.0f, 0.0f, 0.0f };
	float azul[] = { 0.0f, 0.0f, 1.0f };
	float naranja[] = { 0.96f, 0.376f, 0.14f };
	float lila[] = { 0.611f, 0.517f, 0.552f };
	float dark_yellow[] = { 0.831f, 0.686f, 0.215f };
	float negro[] = { 0.0f, 0.0f, 1.0f };
	float silver[] = { 0.752f, 0.752f, 0.752f };
	float light_blue[] = { 0.274f, 0.509f, 0.705f };
	float sky_blue[] = { 0.0f, 0.749f, 1.0f };

	//Todos los planetas apuntan al mismo objeto.
	CuerpoCeleste sol( objetos[0], 1.0f, 0.0f, -9.0f, 0.0f, 0.1f, amarillo );
	CuerpoCeleste mercurio( objetos[0], 0.033f, -0.5f, 0.0f, 0.8f, 0.042, naranja );
	CuerpoCeleste venus( objetos[0], 0.095f, -0.59f, 2.0f, 0.3f, -0.021f, lila );
	CuerpoCeleste tierra( objetos[0], 0.1f, -0.71f, -23.5f, 0.2f, 2.5f, azul );
	CuerpoCeleste marte( objetos[0], 0.053f, -0.8f, -25.0f, 0.1f, 2.5f, rojo );
	CuerpoCeleste jupiter( objetos[0], 0.35f, -1.05f, -3.13f, 0.016f, 6.66f, dark_yellow );
	CuerpoCeleste saturno( objetos[0], 0.3f, -1.4f, -26.73f, 0.006f, 6.0f, silver );
	CuerpoCeleste urano( objetos[0], 0.25f, -1.7f, -97.77f, 0.002f, 3.529f, light_blue );
	CuerpoCeleste neptuno( objetos[0], 0.2f, -2.0f, -28.32f, 0.001f, 3.75f, sky_blue );

	std::vector < CuerpoCeleste* > sistema_solar ;
	sistema_solar.push_back( &sol );
	sistema_solar.push_back( &mercurio );
	sistema_solar.push_back( &venus );
	sistema_solar.push_back( &tierra );
	sistema_solar.push_back( &marte );
	sistema_solar.push_back( &jupiter );
	sistema_solar.push_back( &saturno );
	sistema_solar.push_back( &urano );
	sistema_solar.push_back( &neptuno );

	arma::frowvec eye = {0.0, 0.0, 10.0};
    arma::frowvec camera = {0.0, 0.0, 0.0};

	size_t num_cuerpos = sistema_solar.size();

	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2], 
                camera[0], camera[1], camera[2], 
                0.0, 1.0, 0.0);

		for( int i = 0 ; i < num_cuerpos ; ++i )
		{
			sistema_solar[i]->step();
			DibujaCuerpoCeleste( *sistema_solar[i] );
		}

		glfwSwapBuffers(window);
        glfwPollEvents();

	}	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
}



GLFWwindow* InicializaGLFW()
{
	GLFWwindow* window ;

	if( !glfwInit() )
    {
        fprintf( stderr, "Fallo al inicializar GLFW\n" );
        getchar();
        return( nullptr );
    }

	window = glfwCreateWindow(2048, 768, "Objeto 3D", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "Fallo al abrir la ventana de GLFW.\n" );
        getchar();
        glfwTerminate();
        return( nullptr );
    }

    InicializarCamaraGLFW( window );

	return( window );
}

void InicializarCamaraGLFW( GLFWwindow* _window )
{
	glfwMakeContextCurrent(_window);
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//  Proyecciones
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    float ar = (float)width / height ;
//  Proyección en paralelo
    glViewport(0, 0, width, height);
    glOrtho(-ar, ar, -1.0, 1.0, -20.0, 20.0);
//  Proyección en perspectiva
//    glFrustum(-ar, ar, -ar, ar, 2.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

std::vector < Objeto > LeeObjetos( std::string _nombreArchivo )
{
	clock_t cronometro = clock();

	LectorArchivosOBJ lector ;
	std::vector < Objeto > objetos = lector.LeeObjetosArchivo( std::ifstream( _nombreArchivo ) );

	printf("Lectura de objetos: %.2fs\n", (double)(clock() - cronometro)/CLOCKS_PER_SEC);

	return( objetos );
}

std::vector < arma::frowvec > TransformaObjeto( Objeto& _objeto, arma::fmat _trans )
{
	std::vector < arma::frowvec > vertices_transformados ;
	for( Face cara : _objeto.GetFaces() )
	{
		std::vector< Vertex > triangle_vertex = cara.GetTriangle();
		for( int i = 0 ; i < 3 ; ++i )
		{
			const arma::frowvec vertex = triangle_vertex[i].get_coords();
			arma::fcolvec v = { { vertex[0] }, { vertex[1] }, { vertex[2] }, 1 };
			v = _trans * v ;

			vertices_transformados.push_back( arma::frowvec { v[0], v[1], v[2] } );
		}
	}

	return( std::move( vertices_transformados ) );
}

void DibujaObjeto( std::vector < arma::frowvec >& _vertices, float _color[] )
{
	glColor3f( _color[0], _color[1], _color[2] );
	glBegin(GL_TRIANGLES);
	size_t v_size = _vertices.size();
	for( unsigned int i = 0 ; i < v_size ; ++i )
		glVertex3f( _vertices[i][0], _vertices[i][1], _vertices[i][2] );
			
	glEnd();

	glColor3f( 0.0f, 0.0f, 0.0f );
	glBegin( GL_LINES );
	--v_size ;
	for( unsigned int i = 1 ; i < v_size ; ++i )
	{
		glVertex3f( _vertices[i][0], _vertices[i][1], _vertices[i][2] );
		glVertex3f( _vertices[i+1][0], _vertices[i+1][1], _vertices[i+1][2] );
	}

	glVertex3f( _vertices[v_size][0], _vertices[v_size][1], _vertices[v_size][2] );
	glVertex3f( _vertices[0][0], _vertices[0][1], _vertices[0][2] );

	glEnd();
}

void DibujaCuerpoCeleste( CuerpoCeleste& _c )
{
	std::vector < arma::frowvec > c_vertex = _c.get_actual_vertex();
	DibujaObjeto( c_vertex, _c.get_color() );
}

arma::fmat TransformaPlaneta( arma::fmat _matriz, float _dist_sol, 
								float _grados_trasl, float _grados_tilt, 
								float _grados_rot )
{
	Transform Tr ;
	arma::fmat res = _matriz ;
	res = res * Tr.R( 0.0f, 1.0f, 0.0f, _grados_trasl ) * 
				Tr.T( _dist_sol, 0.0f, 0.0f ) *
				Tr.R( 0.0f, 0.0f, 1.0f, _grados_tilt ) *
				Tr.R( 0.0f, 1.0f, 0.0f, _grados_rot );

	return( std::move( res ) );
}