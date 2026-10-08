//////////////////////////////////////////////////////////////////////////////
//
//  --- textureutils.cpp ---
//
//////////////////////////////////////////////////////////////////////////////

#define STB_IMAGE_IMPLEMENTATION
#include "../common/stb_image.h"	// Sean Barrett's image loader - http://nothings.org/
#include <iostream>

#include "textureutils.h"

//----------------------------------------------------------------------------

void load_texture(const char * filename, GLuint texID, GLint magFilter, GLint minFilter, GLint sWrap, GLint tWrap, bool mipMap, bool invert) {
    int w, h, n;
    int force_channels = 4;
    unsigned char *image_data;

    // Activate unit 0
    glActiveTexture( GL_TEXTURE0 );

    image_data = stbi_load(filename, &w, &h, &n, force_channels);
    if (!image_data) {
        fprintf(stderr, "ERROR: could not load %s\n", filename);
        return;
    }
    printf("Successfully Loaded %s: %d x %d\n", filename, w, h);
    // NPOT check for power of 2 dimensions
    if ((w & (w - 1)) != 0 || (h & (h - 1)) != 0) {
        fprintf(stderr, "WARNING: texture %s is not power-of-2 dimensions\n", filename);
    }
    
    // Invert image (e.g. jpeg, png)
    if (invert) {
        int width_in_bytes = w * 4;
        unsigned char *top = NULL;
        unsigned char *bottom = NULL;
        unsigned char temp = 0;
        int half_height = h / 2;

        for ( int row = 0; row < half_height; row++ ) {
            top = image_data + row * width_in_bytes;
            bottom = image_data + ( h - row - 1 ) * width_in_bytes;
            for ( int col = 0; col < width_in_bytes; col++ ) {
                temp = *top;
                *top = *bottom;
                *bottom = temp;
                top++;
                bottom++;
            }
        }
	}

    // Bind current texture id
    glBindTexture(GL_TEXTURE_2D, texID);
    // Load image data into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image_data);
    // Generate mipmaps for texture
    if (mipMap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    // Set scaling modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    // Set wrapping modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
    // Set maximum anisotropic filtering for system
    GLfloat max_aniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
    // set the maximum!
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
}
