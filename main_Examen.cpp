#pragma once
#include <GLFW/glfw3.h>
#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#endif
#include <GL/glu.h>
#include <armadillo>

//Variables globales--------------------------------
arma::frowvec eye = { 0.0f, 1.0f, 2.0f, 1.0f };
arma::frowvec camera = { 0.0f, 0.0f, 0.0f, 1.0f };
arma::frowvec foco { 0.0f, 1.0f, 0.0f };
//--------------------------------------------------

