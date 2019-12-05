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
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#endif
#include <GL/glu.h>
#include "LectorArchivosOBJ.hpp"
#include "Transform.hpp"
#include "FabricaObstaculos.hpp"
#include "Player.hpp"

//Colores constantes.
float light_blue[] = { 0.274f, 0.509f, 0.705f };
float silver[] = { 0.752f, 0.752f, 0.752f };

//Globales.
int WIND_WIDTH = 1024 ;
int WIND_HEIGHT = 384 ;
arma::frowvec eye = { 0.0f, 1.0f, 2.0f, 1.0f };
float eye_angle = 0.0f ;
arma::frowvec camera = { 0.0f, 0.0f, 0.0f, 1.0f };
float camera_angle = 0.0f ;
arma::frowvec foco { 0.0f, 1.0f, 0.0f };
Player* player ;

//Prototipos------------------------------------------
GLFWwindow* InicializaGLFW();
void InicializarCamaraGLFW( GLFWwindow* _window );
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );
std::vector < arma::frowvec > TransformaObjeto( Objeto& _objeto, const arma::fmat& _trans );
void DibujaObjeto( const std::vector < arma::frowvec >& _vertices, float _color[] );
void DibujaCara( const Face& _caras, float _color[] );
void TeclaPresionada( GLFWwindow* window, int key, int scancode, int action, int mods );
void MuevePlayer( int _accion_tecla, int _movimiento );
void MueveCamara( int _movimiento );
void DibujaEjes();
float Magnitud( const arma::frowvec& _vector );
float ProductoPunto( const arma::frowvec& _v1, const arma::frowvec& _v2 );
std::vector < Face > DameCarasVisibles( const arma::frowvec& _camera, const Objeto& _objeto );
std::vector < Face > DameCarasVisibles( const arma::frowvec& _camera, const std::vector < Face >& _caras );
float DameRazonIluminado( const Face& _cara, const arma::frowvec& _foco );
bool ChecaSiChoco( std::vector < Face >& _player, std::vector < std::vector < Face > >& _obstaculos );
bool ChecaPuntoDentro( const arma::frowvec& _punto, std::vector < Face >& _obst );
float Max( float _1, float _2, float _3 );
float Min( float _1, float _2, float _3 );
//----------------------------------------------------

int main()
{
	std::string nombreArchivo = "player.obj" ;
	std::vector < Objeto > objetos = LeeObjetos( nombreArchivo );
	Objeto media_piramide = objetos[0] ;

	nombreArchivo = "cube.obj" ;
	objetos = LeeObjetos( nombreArchivo );
	Objeto cubo = objetos[0] ;

	GLFWwindow* window ;
	if( ( window = InicializaGLFW()) == nullptr )
		return( -1 );

	//Normalizacion camara y foco.
	arma::frowvec camara_n = camera - eye ;
	camara_n = camara_n / Magnitud( camara_n );
	arma::frowvec foco_n = foco / Magnitud( foco );

	Player temp_p( media_piramide );
	player = &temp_p ;
	FabricaObstaculos f( cubo );

	Transform t ;
	arma::fmat dist_mov_obstaculos = t.T( 0.0f, 0.0f, 0.001f );
	int should_break = 5 ;

	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2], 
                camera[0], camera[1], camera[2], 
                0.0, 1.0, 0.0);

		//Velocidad, bajarle a el param. 0.007 para menor vel.
		if( dist_mov_obstaculos[14] < 0.02 )
			dist_mov_obstaculos[14] += 0.001f ;

		std::vector < Face > caras_player ;
		for( auto cara : player->dame_caras_transf() )
		{
			float razon_color = DameRazonIluminado( cara, foco_n );
			float color[] { light_blue[0] * razon_color, light_blue[1] * razon_color, light_blue[2] * razon_color };
			DibujaCara( cara, color );
			caras_player.push_back( cara );
		}

		std::vector < std::vector < Face > > caras_obstaculos ; 
		for( Obstaculo obs : f.dame_obstaculos() )
		{	
			std::vector < Face > caras_obst_actual ;
			for( auto cara : obs.dame_caras_transf() )
			{
				float razon_color = DameRazonIluminado( cara, foco_n );
				float color[] { silver[0] * razon_color, silver[1] * razon_color, silver[2] * razon_color };
				DibujaCara( cara, color );
				caras_obst_actual.push_back( cara );
			}

			caras_obstaculos.push_back( std::move( caras_obst_actual ) );
		}

		if( ChecaSiChoco( caras_player, caras_obstaculos ) )
			should_break-- ;

		f.avanza( dist_mov_obstaculos );

		DibujaEjes();
		glfwSwapBuffers(window);
        glfwPollEvents();

	}	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 && should_break > 0 );
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
    /*glViewport(0, 0, width, height);
    glOrtho(-ar, ar, -1.0, 1.0, -20.0, 20.0);*/
//  Proyección en perspectiva
	glFrustum(-ar, ar, -ar, ar, 1.0, 5.0);
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

std::vector < arma::frowvec > TransformaObjeto( Objeto& _objeto, const arma::fmat& _trans )
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

void DibujaCara( const Face& _cara, float _color[] )
{
		std::vector < Vertex > vertices = _cara.GetTriangle();
		glColor3f( _color[0], _color[1], _color[2] );
		glBegin(GL_TRIANGLES);
		for( int i = 0 ; i < 3 ; ++i )
			glVertex3f( vertices[i].get_coords()[0], vertices[i].get_coords()[1], vertices[i].get_coords()[2] );
		glEnd();
}

void TeclaPresionada( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if( action == 1 || action == 2 )
	{
		if( key == 262 )	//Right
			player->ladea( 1 );

		else if( key == 263 )	//Left
			player->ladea( -1 );

		else if( key == 65 )
			MueveCamara( -1 );

		else if( key == 68 )
			MueveCamara( 1 );
	}
}

void MueveCamara( int _movimiento )
{
	Transform t ;
	eye_angle = eye_angle + _movimiento > 360.f ? 0.0f : eye_angle + _movimiento ;
	arma::fmat eye_transf = t.R( 0.0f, 1.0f, 0.0f, eye_angle ) * t.T( 0.0f, 1.0f, 2.0f );
	arma::fcolvec temp_eye = eye_transf * arma::fcolvec( { {0}, {0}, {0}, {1} } );

	eye = { temp_eye[0], temp_eye[1], temp_eye[2], 1.0f };
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
	glVertex3f( -50.0f, 0.0f , -2.9f );
	glVertex3f( 50.0f, 0.0f , -2.9f );
	/*glVertex3f( -10.0f, 0.0f , 0.0f );
	glVertex3f( 10.0f, 0.0f , 0.0f );
	glVertex3f( 0.0f, -10.0f , 0.0f );
	glVertex3f( 0.0f, 10.0f , 0.0f );
	glVertex3f( 0.0f, 0.0f , -10.0f );
	glVertex3f( 0.0f, 0.0f , 10.0f );*/
	glEnd();
}

float Magnitud( const arma::frowvec& _vector )
{
	float magnitud = sqrtf( powf( _vector[0], 2 ) + powf( _vector[1], 2 ) + powf( _vector[2], 2 ) );
	return( magnitud );
}

float ProductoPunto( const arma::frowvec& _v1, const arma::frowvec& _v2 )
{
	float x_r = _v1[0] * _v2[0] ;
	float y_r = _v1[1] * _v2[1] ;
	float z_r = _v1[2] * _v2[2] ;

	return( x_r + y_r + z_r );
}

std::vector < Face > DameCarasVisibles( const arma::frowvec& _camera, const Objeto& _objeto )
{
	std::vector < Face > normalesVisibles ;
	for( const Face f : _objeto.GetFaces() )
	{
		arma::frowvec normalnorm = f.dame_normal_normalizada();
		float cos_theta = ProductoPunto( normalnorm, _camera );

		if( cos_theta < 0.0f )
			normalesVisibles.push_back( f );
	}

	return( normalesVisibles );
}

std::vector < Face > DameCarasVisibles( const arma::frowvec& _camera, const std::vector < Face >& _caras )
{
	std::vector < Face > normalesVisibles ;
	for( const Face f : _caras )
	{
		auto triang = f.GetTriangle();
		arma::frowvec normal = arma::cross( triang[1].get_coords() - triang[0].get_coords(), 
										triang[2].get_coords() - triang[0].get_coords() );

		auto mag = sqrtf( powf( normal[0], 2 ) + powf( normal[1], 2 ) + powf( normal[2], 2 ) );

		normal = normal / mag ;

		float cos_theta = ProductoPunto( normal, _camera );

		if( cos_theta < 0.0f )
			normalesVisibles.push_back( f );
	}

	return( normalesVisibles );
}

float DameRazonIluminado( const Face& _cara, const arma::frowvec& _foco )
{
	float Ia = 0.95f ;
	float Ka = 0.58f ;
	float Ip = 0.8f ;
	float Kd = 0.75f ;

	auto triang = _cara.GetTriangle();
	arma::frowvec normal = arma::cross( triang[1].get_coords() - triang[0].get_coords(), 
									triang[2].get_coords() - triang[0].get_coords() );

	auto mag = sqrtf( powf( normal[0], 2 ) + powf( normal[1], 2 ) + powf( normal[2], 2 ) );

	normal = normal / mag ;

	float cos_theta = ProductoPunto( normal, _foco );
	float I = ( Ia * Ka ) + ( Ip * Kd * cos_theta );

	return( I / 2 );
}

bool ChecaSiChoco( std::vector < Face >& _player, std::vector < std::vector < Face > >& _obstaculos )
{
	for( Face cara : _player )
	{
		std::vector < Vertex > vertices = cara.GetTriangle();
		for( Vertex v : vertices )
		{
			for( auto obs : _obstaculos )
			{
				if( ChecaPuntoDentro( v.get_coords(), obs ) )
					return( true );
			}
		}
	}

	return( false );
}

bool ChecaPuntoDentro( const arma::frowvec& _punto, std::vector < Face >& _obst )
{
	int numero_veces_intersecta = 0 ;
	float last_x = 0 ;
	for( Face f : _obst )
	{
		std::vector < Vertex > coords = f.GetTriangle();
		arma::frowvec p1 = coords[0].get_coords();
		arma::frowvec p2 = coords[1].get_coords();
		arma::frowvec p3 = coords[2].get_coords();

		if( p1.at(2) == p2.at(2) && p1.at(2) == p3.at(2) &&
				p1.at( 2 ) > 0.79 )
		{
			if( _punto[2] > p1[2] )
			{
				float max_x = Max( p1[0], p2[0], p3[0] );
				float min_x = Min( p1[0], p2[0], p3[0] );

				if( _punto[0] < max_x && _punto[0] > min_x )
				{
					if( _punto[0] != last_x )
					{
						last_x = _punto[0] ;
						numero_veces_intersecta++ ;
					}
				}
			}
		}
	}

	if( numero_veces_intersecta % 2 == 0 )
		return( false );

	return( true  );
}

float Max( float _1, float _2, float _3 )
{
	if( _1 > _2 )
	{
		if( _1 > _3 )
			return( _1 );

		else
			return( _3 );
	}

	else if( _2 > _3 )
		return( _2 );

	return(  _3 ); 
}

float Min( float _1, float _2, float _3 )
{
	if( _1 < _2 )
	{
		if( _1 < _3 )
			return( _1 );

		else
			return( _3 );
	}

	else if( _2 < _3 )
		return( _2 );

	return(  _3 ); 
}

