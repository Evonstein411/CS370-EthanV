#include "kitUI/hud.h"

extern SceneObject* selectedObject;
extern void createOutlineFBO(int width, int height);

vec3 dragStartPos;
vec2 dragStartMouse;

//functions
void cursor_to_framebuffer(GLFWwindow* window, double xpos, double ypos, float& mx, float& my);
void handle_key_ortho(int key, int action);
void handle_key_perspective(int key, int action);

//keyboard callback
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if(hud_key(key, action)) {
        return;
    }

    //deselect with escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        selectedObject = nullptr;
        update_fields();
    }

    //delete with delete key
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        delete_selected_object();
        return;
    }


    if (camera_mode == ORTHO) {
        handle_key_ortho(key, action);
    }else {
        handle_key_perspective(key, action);
    }

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

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        SceneObject* clicked = pick_scene_object(mx, my);
        int raw = get_raw_index();   // or rawIndex if it’s global

        if ((raw == GIZMO_X || raw == GIZMO_Y || raw == GIZMO_Z) && selectedObject) {
            activeGizmo = (GizmoAxis)raw;
            dragStartPos = selectedObject->position;
            dragStartMouse = vec2(mx, my);
            return;
        }


        select_object(clicked);
        activeGizmo = GIZMO_NONE;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        activeGizmo = GIZMO_NONE;
    }


}


void char_callback(GLFWwindow*, unsigned int codepoint) {
    hud_char(codepoint);
}

//mouse move callback
void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    float mx, my;
    cursor_to_framebuffer(window, xpos, ypos, mx, my);
    hud_cursor(mx, my);


    if (activeGizmo == GIZMO_NONE || !selectedObject) return;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        activeGizmo = GIZMO_NONE;
        return;
    }

    vec2 delta = vec2(mx, my) - dragStartMouse;
    float sensitivity = 0.0025f * radius;   // tune

    float amount = 0.0f;
    vec3 axis(0.0f);

    if (activeGizmo == GIZMO_X) {
        amount =  delta[0] * sensitivity;
        axis   = vec3(1, 0, 0);
    }
    else if (activeGizmo == GIZMO_Y) {
        amount = delta[1] * sensitivity;
        axis   = vec3(0, 1, 0);
    }
    else if (activeGizmo == GIZMO_Z) {
        amount =  delta[0] * sensitivity;
        axis   = vec3(0, 0, 1);
    }

    selectedObject->position = dragStartPos + axis * amount;
    update_fields();
}


//update on window resize
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    ww = width;
    hh = height;
    createOutlineFBO(ww, hh);
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

void handle_key_ortho(int key, int action) {
    // A/D Adjust azimuth
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
        azimuth += daz;
        if (azimuth > 360.0) {
            azimuth -= 360.0;
        }
    } else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
        azimuth -= daz;
        if (azimuth < 0.0)
        {
            azimuth += 360.0;
        }
    }

    // W/S Adjust elevation angle
    if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
    {
        elevation += del;
        if (elevation > 180.0)
        {
            elevation = 179.0;
        }
    }
    else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
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

    GLfloat x, y, z;
    x = (GLfloat)(radius*sin(deg2rad(azimuth))*sin(deg2rad(elevation)));
    y = (GLfloat)(radius*cos(deg2rad(elevation)));
    z = (GLfloat)(radius*cos(deg2rad(azimuth))*sin(deg2rad(elevation)));
    eye = vec3(x,y,z);
}

void handle_key_perspective(int key, int action) {
    if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
        fpsAzimuth -= fpsTurn;
        if (fpsAzimuth > 360.0f) fpsAzimuth -= 360.0f;
    } else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
        fpsAzimuth += fpsTurn;
        if (fpsAzimuth < 0.0f) fpsAzimuth += 360.0f;
    }

    // Z/X – look up/down (elevation)
    if (key == GLFW_KEY_Z || key == GLFW_KEY_UP) {
        fpsElevation += fpsTurn;
        if (fpsElevation > 89.0f) fpsElevation = 89.0f;
    } else if (key == GLFW_KEY_X || key == GLFW_KEY_DOWN) {
        fpsElevation -= fpsTurn;
        if (fpsElevation < -89.0f) fpsElevation = -89.0f;
    }

    // W/S – move forward/backward in current direction
    if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
        float yaw   = deg2rad(fpsAzimuth);
        float pitch = deg2rad(fpsElevation);
        vec3 dir = normalize(vec3(
            cos(yaw) * cos(pitch),
            sin(pitch),
            sin(yaw) * cos(pitch)
        ));
        float sign = (key == GLFW_KEY_W) ? 1.0f : -1.0f;
        eye += dir * fpsStep * sign;
    }
}
