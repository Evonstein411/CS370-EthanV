//////////////////////////////////////////////////////////////////////////////
//
//  --- GLFWutils.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __GLFWUTILS_H__
#define __GLFWUTILS_H__

#include "../include/GLEW/glew.h"
#include "../include/GLFW/glfw3.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

//----------------------------------------------------------------------------
//
//  CreateWindow() creates an OpenGL window using GLFW.
//
//  CreateWindow() returns a pointer to a GLFW window with an OpenGL context. 
//

GLFWwindow* CreateWindow( const char *name, GLsizei width, GLsizei height );

//----------------------------------------------------------------------------


#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __GLFWUTILS_H__
