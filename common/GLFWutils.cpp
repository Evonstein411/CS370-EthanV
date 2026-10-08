//////////////////////////////////////////////////////////////////////////////
//
//  --- GLFWutils.cpp ---
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GLFWutils.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


GLFWwindow* CreateWindow(const char *name, GLsizei width, GLsizei height) {
	GLFWwindow* window = NULL;
    const GLubyte *renderer;
    const GLubyte *version;
    /* start GL context and O/S window using the GLFW helper library */
    if ( !glfwInit() ) {
        fprintf( stderr, "ERROR: could not start GLFW3\n" );
        return window;
    }

	// Try to make OpenGL 4.1 core context
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    window = glfwCreateWindow( width, height, name, NULL, NULL );

	// Try to make any OpenGL core context
    if ( !window ) {
		glfwDefaultWindowHints();
    	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
   	 	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    	window = glfwCreateWindow( width, height, name, NULL, NULL );
    }

	// Try to make any OpenGL context
    if ( !window ) {
		glfwDefaultWindowHints();
    	window = glfwCreateWindow( width, height, name, NULL, NULL );
    }

	// Total failure
    if ( !window ) {
        return window;
    }
    glfwMakeContextCurrent( window );

    /* start GLEW extension handler */
    glewExperimental = GL_TRUE;
    glewInit();

    /* get version info */
    renderer = glGetString( GL_RENDERER ); /* get renderer string */
    version = glGetString( GL_VERSION );	 /* version as a string */
    printf( "Renderer: %s\n", renderer );
    printf( "OpenGL version supported: %s\n", version );
    printf( "Created window of size: %d x %d\n", width, height );

    return window;
}


//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus


