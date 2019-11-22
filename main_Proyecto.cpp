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
#include "FabricaObstaculos.hpp"

//Colores constantes.
float light_blue[] = { 0.274f, 0.509f, 0.705f };
float silver[] = { 0.752f, 0.752f, 0.752f };

//Globales.
int WIND_WIDTH = 1024 ;
int WIND_HEIGHT = 384 ;
arma::fcolvec eye = { 0.0f, 10.0f, 10.0f, 1.0f };
float eye_angle = 0.0f ;
arma::fcolvec camera = { 0.0f, 0.0f, 0.0f, 1.0f };
float camera_angle = 0.0f ;

//Prototipos------------------------------------------
GLFWwindow* InicializaGLFW();
void InicializarCamaraGLFW( GLFWwindow* _window );
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );
std::vector < arma::frowvec > TransformaObjeto( Objeto& _objeto, arma::fmat _trans );
void DibujaObjeto( const std::vector < arma::frowvec >& _vertices, float _color[] );
void TeclaPresionada( GLFWwindow* window, int key, int scancode, int action, int mods );
void MuevePlayer( int _accion_tecla, int _movimiento );
void MueveCamara( int _movimiento );
void DibujaEjes();
//----------------------------------------------------

int main()
{
	std::string nombreArchivo = "player.obj" ;
	std::vector < Objeto > objetos = LeeObjetos( nombreArchivo );
	Objeto player = objetos[0] ;

	nombreArchivo = "cube.obj" ;
	objetos = LeeObjetos( nombreArchivo );
	Objeto cubo = objetos[0] ;

	GLFWwindow* window ;
	if( ( window = InicializaGLFW()) == nullptr )
		return( -1 );

	Transform t ;
	arma::fmat transPlayer = t.S( 0.1f, 0.1f, 0.1f ) * t.T( 0.0f, 0.0f, 10.0f );
	auto vertPlayer = TransformaObjeto( player, transPlayer );

	FabricaObstaculos f( cubo );
	arma::fmat dist_mov_obstaculos = t.T( 0.0f, 0.0f, 0.01f );

	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2], 
                camera[0], camera[1], camera[2], 
                0.0, 1.0, 0.0);

		DibujaObjeto( vertPlayer, light_blue );
		for( Obstaculo obs : f.dame_obstaculos() )
		{
			std::vector < arma::frowvec > v = obs.dame_posicion();
			DibujaObjeto( v, silver );
		}

		f.avanza( dist_mov_obstaculos );

		DibujaEjes();
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

void DibujaObjeto( const std::vector < arma::frowvec > & _vertices, float _color[] )
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
	if( action == 1 || action == 2 )
	{
		if( key == 262 )	//Right
			MueveCamara( 1 );

		else if( key == 263 )	//Left
			MueveCamara( -1 );
	}
}

void MueveCamara( int _movimiento )
{
	Transform t ;
	eye_angle = eye_angle + _movimiento > 360.f ? 0.0f : eye_angle + _movimiento ;
	arma::fmat eye_transf = t.R( 0.0f, 1.0f, 0.0f, eye_angle ) * t.T( 0.0f, 10.0f, 10.0f );
	eye = eye_transf * arma::fcolvec( { {0}, {0}, {0}, {1} } );
}

void MuevePlayer( int _accion_tecla, int _movimiento )
{
	//Mover al main
	if( _accion_tecla == 1 )
	{
		Transform t ;
		eye_angle += 5.0f ;
		arma::fmat eye_transf = t.T( eye[0], eye[1], eye[2] );
		arma::fcolvec eye_col = { { 0.0 }, { 0.0 }, { 0.0 }, 1 } ;
		eye_transf = t.R( 0.0f, 1.0f, 0.0f, _movimiento * eye_angle ) * eye_transf ;
		auto eye_2 = eye_transf * eye_col ;
	}
}

void DibujaEjes()
{
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_LINES );
	glVertex3f( -10.0f, 0.0f , 0.0f );
	glVertex3f( 10.0f, 0.0f , 0.0f );
	glVertex3f( 0.0f, -10.0f , 0.0f );
	glVertex3f( 0.0f, 10.0f , 0.0f );
	glVertex3f( 0.0f, 0.0f , -10.0f );
	glVertex3f( 0.0f, 0.0f , 10.0f );
	glEnd();
}

