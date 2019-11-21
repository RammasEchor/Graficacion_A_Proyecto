//Sistema solar.
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

//Global
int WIND_WIDTH = 1024 ;
int WIND_HEIGHT = 384 ;

//Prototipos------------------------------------------
GLFWwindow* InicializaGLFW();
void InicializarCamaraGLFW( GLFWwindow* _window );
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );
std::vector < arma::frowvec > TransformaObjeto( Objeto& _objeto, arma::fmat _trans );
void DibujaObjeto( std::vector < arma::frowvec >& _vertices, float _color[] );
void TeclaPresionada( GLFWwindow* window, int key, int scancode, int action, int mods );
//----------------------------------------------------

int main()
{
	std::string nombreArchivo = "player.obj" ;
	std::vector < Objeto > objetos = LeeObjetos( nombreArchivo );

	GLFWwindow* window ;
	if( ( window = InicializaGLFW()) == nullptr )
		return( -1 );

	arma::frowvec eye = {0.0, 10.0, 10.0};
    arma::frowvec camera = {0.0, -10.0, -10.0};

	float light_blue[] = { 0.274f, 0.509f, 0.705f };

	Transform t ;
	arma::fmat trans = t.S( 0.1f, 0.1f, 0.1f ) * t.T( 0.0f, 0.0f, 10.0f );
	auto vertices = TransformaObjeto( objetos[0], trans );

	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2], 
                camera[0], camera[1], camera[2], 
                0.0, 1.0, 0.0);

		DibujaObjeto( vertices, light_blue );

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

	window = glfwCreateWindow(730, 512, "Objeto 3D", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "Fallo al abrir la ventana de GLFW.\n" );
        getchar();
        glfwTerminate();
        return( nullptr );
    }

    InicializarCamaraGLFW( window );
	glfwSetKeyCallback( window, TeclaPresionada );

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

void TeclaPresionada( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if( key == 262 )	//Right
		MuevePlayer( action, 1 );

	else if( key == 263 )	//Left
		MuevePlayer( action, -1 );
}

void MuevePlayer( int _accion_tecla, int _movimiento )
{
	
}