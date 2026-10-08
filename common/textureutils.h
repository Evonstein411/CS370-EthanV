//////////////////////////////////////////////////////////////////////////////
//
//  --- textureutils.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __TEXTUREUTILS_H__
#define __TEXTUREUTILS_H__

#include "vgl.h"


//----------------------------------------------------------------------------
//
//  loadtexture() takes an image filename, texture id, and texture map parameters
//	              and creates a texture map from the image file
//
//  	filename - name of image file (bmp, jpg, or png preferred)
//  	texID - texture ID for the texture map
//  	magFilter - magnification filter GL constant
//  	minFilter - minification filter GL constant
//  	sWrap - horizontal wrapping mode GL constant
//  	tWrap - vertical wrapping mode GL constant
//  	mipMap - flag to autogenerate mipmaps
//  	invert - flag to flip image vertically in texture map
//

void load_texture(const char * filename, GLuint texID, GLint magFilter, GLint minFilter, GLint sWrap, GLint tWrap, bool mipMap, bool invert);

#endif // __TEXTUREUTILS_H__
