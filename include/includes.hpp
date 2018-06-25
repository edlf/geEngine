/*
 * Eduardo Fernandes
 *
 * Includes globais e defines.
 */

#ifndef INCLUDES_HPP_
#define INCLUDES_HPP_

#define APP_NAME_VERSION  "geEngine v0.6"

/* Remove the comment bellow to see the engine output */
//#define ENGINE_VERBOSE
/* OpenGL light max */
#define MAX_LIGHTS 8

/* Slightly faster than the 60hz */
#define GLUT_UPDATE_MS 16

/* buffer size for the picking */
#define PICKING_BUFFER_SIZE 256

#define WINDOW_SIZE_X 800
#define WINDOW_SIZE_Y 600
#define WINDOW_INITIALPOS_X 50
#define WINDOW_INITIALPOS_Y 50

// #define GLEW_STATIC
// #define FREEGLUT_STATIC

#define STRING_CONSOLE_LINE          "-------------------------------------------------------------------------------"
#define STRING_CONSOLE_HEADER_SPACER "                          "

/* GUI camera stuff, once again stolen from CGFlib (divided values by 2 for more accuracy) */
const double MOUSE_ROTATE_FACTOR = 0.25;
const double MOUSE_PAN_FACTOR = 0.025;
const double MOUSE_ZOOM_FACTOR = 0.25;

#define MAX_DISPLAY_LISTS 64

/* Folders */
#define TEXTURE_FOLDER "textures/"
#define SHADER_FOLDER  "shaders/"

/* Std c++ includes */
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <limits>
#include <stack>

/* OpenGL includes */
extern "C" {
#include <GL/glew.h>
}
#include <GL/freeglut.h>
#include <GL/glui.h>

#include <types.hpp>
#include <Exception.hpp>

/* Win32 Hacks */
#ifdef WIN32
#define M_PI   3.14159265358979323846264338327950288
#endif

#endif

