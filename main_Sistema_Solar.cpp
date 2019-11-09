//Lee y muestra los objetos leidos.
//
#pragma once
#include <iterator>
#include <time.h>
#include <regex>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "LectorArchivos.hpp"
#include "Transform.hpp"

//Prototipos------------------------------------------
GLFWwindow* InicializaGLFW();
void InicializarCamaraGLFW( GLFWwindow* _window );
std::vector < Objeto > LeeObjetos( std::string _nombreArchivo );
std::vector < arma::frowvec > TransformaObjeto( Objeto _objeto, arma::fmat _trans );
void DibujaObjeto( std::vector < arma::frowvec > _vertices, float _color[] );
arma::fmat TransformaPlaneta( arma::fmat _matriz, float _dist_sol, 
								float _grados_trasl, float _grados_tilt, 
								float _grados_rot );
//----------------------------------------------------

int main()
{
	std::string nombreArchivo = "Modelos/roca.obj" ;
	std::vector < Objeto > objetos = LeeObjetos( nombreArchivo );

	GLFWwindow* window ;
	if( ( window = InicializaGLFW()) == nullptr )
		return( -1 );

	//Todos los planetas apuntan al mismo objeto.
	Objeto sol = objetos[0] ;
	Objeto tierra = objetos[0] ;

	arma::frowvec eye = {0.0, 0.0, 10.0};
    arma::frowvec camera = {0.0, 0.0, 0.0};

	Transform Tr = Transform();

	//Matrices de escalación.
	float escala_sol = 1.0f ;
	float escala_tierra = 0.2f ;
	arma::fmat transf_inicial_sol = Tr.S(escala_sol, escala_sol, escala_sol);
	arma::fmat transf_inicial_tierra = Tr.S( escala_tierra, escala_tierra, escala_tierra );
	
	//Angulos de los planetas y el sol.
	float angulo_rot_sol = 0.0f ;

    float angulo_rot_tierra = 0.0f ;
	float angulo_trasl_tierra = 0.0f ;
	float angulo_tilt_tierra = 23.5f ;

	//Colores
	float amarillo[] = { 1.0f, 1.0f, 0.0f };
	float rojo[] = { 1.0f, 0.0f, 0.0f };
	float azul[] = { 0.0f, 0.0f, 1.0f };
	float negro[] = { 0.0f, 0.0f, 1.0f };

	do{
		arma::fmat transf_sol = transf_inicial_sol ;
		arma::fmat transf_tierra = transf_inicial_tierra ;

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(eye[0], eye[1], eye[2], 
                camera[0], camera[1], camera[2], 
                0.0, 1.0, 0.0);

		//Actualización de transformaciones
		//Sol
		transf_sol = TransformaPlaneta( transf_sol, 0.0f, 0.0f, -9.0f, angulo_rot_sol );

		angulo_rot_sol = (angulo_rot_sol < 360.0f) ? angulo_rot_sol + 0.1f : 0.0f;

		//Tierra
        transf_tierra = TransformaPlaneta( transf_tierra, -5.5f, angulo_trasl_tierra,
											angulo_tilt_tierra, angulo_rot_tierra );

		angulo_rot_tierra = (angulo_rot_tierra < 360.0f) ? angulo_rot_tierra + 2.5f : 0.0f;
		angulo_trasl_tierra = (angulo_trasl_tierra < 360.0f) ? angulo_trasl_tierra + 0.1f : 0.0f;

		//Aplicación de la matriz de transformación.
		std::vector < arma::frowvec > sol_transformado = TransformaObjeto( sol, transf_sol );
		std::vector < arma::frowvec > tierra_transformada = TransformaObjeto( tierra, transf_tierra );
		
		//Dibujado.
		DibujaObjeto( sol_transformado, amarillo );
		DibujaObjeto( tierra_transformada, azul );

		glfwSwapBuffers(window);
        glfwPollEvents();

	}	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
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

GLFWwindow* InicializaGLFW()
{
	GLFWwindow* window ;

	if( !glfwInit() )
    {
        fprintf( stderr, "Fallo al inicializar GLFW\n" );
        getchar();
        return( nullptr );
    }

	window = glfwCreateWindow(1024, 768, "Objeto 3D", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "Fallo al abrir la ventana de GLFW.\n" );
        getchar();
        glfwTerminate();
        return( nullptr );
    }

    InicializarCamaraGLFW( window );

	return( window );
}

std::vector < Objeto > LeeObjetos( std::string _nombreArchivo )
{
	clock_t cronometro = clock();

	LectorArchivosOBJ lector ;
	std::vector < Objeto > objetos = lector.LeeObjetosArchivo( std::ifstream( _nombreArchivo ) );

	printf("Lectura de objetos: %.2fs\n", (double)(clock() - cronometro)/CLOCKS_PER_SEC);

	return( objetos );
}

std::vector < arma::frowvec > TransformaObjeto( Objeto _objeto, arma::fmat _trans )
{
	std::vector < arma::frowvec > vertices_transformados ;
	for( Face cara : _objeto.GetFaces() )
	{
		std::vector< Vertex > triangle_vertex = cara.GetTriangle();
		for( int i = 0 ; i < 3 ; ++i )
		{
			auto x = triangle_vertex[i].GetCoords()[0];
			auto y = triangle_vertex[i].GetCoords()[1];
			auto z = triangle_vertex[i].GetCoords()[2];

			arma::fcolvec v = { { x }, { y }, { z }, 1 };
			v = _trans * v ;

			vertices_transformados.push_back( arma::frowvec { v[0], v[1], v[2] } );
		}
	}

	return( std::move( vertices_transformados ) );
}

void DibujaObjeto( std::vector < arma::frowvec > _vertices, float _color[] )
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