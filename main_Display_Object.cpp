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
#include "LectorArchivos.hpp"
#include "Transform.hpp"

void DisplayObject( GLFWwindow* window, const std::vector < Objeto >& _objetos );

int main()
{
	std::string nombre = "Modelos/roca.obj" ;

	clock_t cronometro = clock();

	LectorArchivosOBJ lector ;
	std::vector < Objeto > objetos = lector.LeeObjetosArchivo( std::ifstream( nombre ) );

	printf("Time taken: %.2fs\n", (double)(clock() - cronometro)/CLOCKS_PER_SEC);

	GLFWwindow* window ;

	if( !glfwInit() )
    {
        fprintf( stderr, "Fallo al inicializar GLFW\n" );
        getchar();
        return -1;
    }

	window = glfwCreateWindow(1024, 768, "Objeto 3D", NULL, NULL);
    if( window == NULL ) {
        fprintf( stderr, "Fallo al abrir la ventana de GLFW.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	DisplayObject( window, objetos );
}

void DisplayObject( GLFWwindow* window, const std::vector < Objeto >& _objetos )
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	do {
        glClear( GL_COLOR_BUFFER_BIT );
		for( Objeto objeto : _objetos )
		{
			float div = 1 / objeto.top ;
			
			for( Face cara : objeto.GetFaces() )
			{
				std::vector< Vertex > triangle_vertex = cara.GetTriangle();
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_TRIANGLES);
					for( int i = 0 ; i < 3 ; ++i )
					{
							glVertex3f(	triangle_vertex[i].GetCoords()[0] * div, 
										triangle_vertex[i].GetCoords()[1] * div, 
										triangle_vertex[i].GetCoords()[2] *	div	);
					}
				glEnd();
			}

			for( Face cara : objeto.GetFaces() )
			{
				for( Edge arista : cara.GetEdges() )
				{
					Vertex vi = arista.GetInitialVertex();
					Vertex vf = arista.GetFinalVertex();
					glColor3f(0.0, 0.0, 0.0);
					glBegin( GL_LINES );
						glVertex3f( vi.GetCoords()[0] * div, 
									vi.GetCoords()[1] * div, 
									vi.GetCoords()[2] * div );

						glVertex3f( vf.GetCoords()[0] * div, 
									vf.GetCoords()[1] * div, 
									vf.GetCoords()[2] * div );
					glEnd();
				}
			}
		}
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );
}


