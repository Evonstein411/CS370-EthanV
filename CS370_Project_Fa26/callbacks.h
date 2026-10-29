#include "kitUI/hud.h"

extern SceneObject* selectedObject;


//functions
void cursor_to_framebuffer(GLFWwindow* window, double xpos, double ypos, float& mx, float& my);


//keyboard callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if(hud_key(key, action)) {
        return;
    }

    
    // A/D Adjust azimuth
    if (key == GLFW_KEY_A) {
        azimuth += daz;
        if (azimuth > 360.0) {
            azimuth -= 360.0;
        }
    } else if (key == GLFW_KEY_D) {
        azimuth -= daz;
        if (azimuth < 0.0)
        {
            azimuth += 360.0;
        }
    }

    // W/S Adjust elevation angle
    if (key == GLFW_KEY_W)
    {
        elevation += del;
        if (elevation > 180.0)
        {
            elevation = 179.0;
        }
    }
    else if (key == GLFW_KEY_S)
    {
        elevation -= del;
        if (elevation < 0.0)
        {
            elevation = 1.0;
        }
    }

    // X/Z Adjust radius (zoom)
    if (key == GLFW_KEY_X)
    {
        radius += dr;
    }
    else if (key == GLFW_KEY_Z)
    {
        radius -= dr;
        if (radius < min_radius)
        {
            radius = min_radius;
        }
    }

    // Compute updated camera position
    GLfloat x, y, z;
    x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
    y = (GLfloat)(radius*cos(deg2rad(elevation)));
    z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
    eye = vec3(x,y,z);
}

//mouse callback
void mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    //update mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float mx, my;
    cursor_to_framebuffer(window, xpos, ypos, mx, my);

    if (hud_mouse_button(mx, my, button, action))
        return;   // UI ate it



    //check if clicked on object
    SceneObject* clicked = pick_scene_object(mx, my);
    selectedObject = clicked;
    update_fields();


}


void char_callback(GLFWwindow*, unsigned int codepoint) {
    hud_char(codepoint);
}

//mouse move callback
void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    float mx, my;
    cursor_to_framebuffer(window, xpos, ypos, mx, my);
    hud_cursor(mx, my);
}


//update on window resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    ww = width;
    hh = height;
}

//ensure cursor matching size of window and proper scaling
void cursor_to_framebuffer(GLFWwindow* window, double xpos, double ypos, float& mx, float& my) {
    int win_w, win_h;
    glfwGetWindowSize(window, &win_w, &win_h);
    if (win_w <= 0 || win_h <= 0) {
        mx = (float)xpos;
        my = (float)hh - (float)ypos;
        return;
    }
    float xscale = (float)ww / (float)win_w;
    float yscale = (float)hh / (float)win_h;
    mx = (float)xpos * xscale;
    my = (float)hh - (float)ypos * yscale;
}
