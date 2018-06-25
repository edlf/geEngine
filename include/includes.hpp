/*
 * Eduardo Fernandes
 *
 * Includes globals e defines.
 */

#ifndef INCLUDES_HPP_
#define INCLUDES_HPP_

/* OpenGL light max */
#define MAX_LIGHTS 8

/* Slightly faster than the 60hz */
#define GLUT_UPDATE_MS 16

/* buffer size for the picking */
#define PICKING_BUFFER_SIZE 256

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

