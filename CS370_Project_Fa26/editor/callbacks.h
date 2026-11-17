#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "hitdetection.h"
#include "../kitUI/hud.h"

extern void create_outline_fbo(int width, int height);

inline vec3 dragStartPos;
inline vec2 dragStartMouse;
inline bool rightMouseDown = false;
inline double lastMouseX = 0.0;
inline double lastMouseY = 0.0;
constexpr float mouseLookSensitivity = 0.2f;

//functions
void cursor_to_framebuffer(GLFWwindow* window, double xPos, double yPos, float& mx, float& my);
void handle_key_ortho(int key, int action);
void handle_key_perspective(int key, int action);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

//keyboard callback
inline void key_callback(GLFWwindow *window, const int key, int scancode, const int action, int mods) {

    if(hud_key(key, action)) {
        return;
    }

    //deselect with escape
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        sceneManager.set_selected(nullptr);
        update_fields();
    }

    //delete with delete key
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        delete_selected_object();
        return;
    }


    if (cameraMode == ORTHO) {
        handle_key_ortho(key, action);
    }else {
        handle_key_perspective(key, action);
    }

}

//mouse callback
inline void mouse_callback(GLFWwindow *window, const int button, const int action, int mods) {
    //update mouse position
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    float mx, my;
    cursor_to_framebuffer(window, xPos, yPos, mx, my);

    if (hud_mouse_button(mx, my, button, action))
        return;   // UI ate it

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            rightMouseDown = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            rightMouseDown = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        SceneNode* clicked = pick_scene_object(mx, my);
        int raw = get_raw_index();   // or rawIndex if it’s global

        if (const SceneNode* selected = sceneManager.get_selected(); (raw == GIZMO_X || raw == GIZMO_Y || raw == GIZMO_Z) && selected) {
            activeGizmo = static_cast<GizmoAxis>(raw);
            dragStartPos = selected->position;
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


inline void char_callback(GLFWwindow*, const unsigned int codepoint) {
    hud_char(codepoint);
}

//mouse move callback
inline void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    float mx, my;
    cursor_to_framebuffer(window, xpos, ypos, mx, my);
    hud_cursor(mx, my);

    if (rightMouseDown) {
        glfwGetCursorPos(window, &xpos, &ypos);

        const auto dx = static_cast<float>(xpos - lastMouseX);
        const auto dy = static_cast<float>(ypos - lastMouseY);
        lastMouseX = xpos;
        lastMouseY = ypos;

        if (cameraMode == ORTHO) {
            azimuth   -= dx * mouseLookSensitivity;
            elevation -= dy * mouseLookSensitivity;

            // clamp / wrap the same way the keyboard does
            if (azimuth > 360.0f) azimuth -= 360.0f;
            if (azimuth < 0.0f)   azimuth += 360.0f;
            if (elevation > 179.0f) elevation = 179.0f;
            if (elevation < 1.0f)   elevation = 1.0f;

            // recompute eye from spherical coords (same as handle_key_ortho)
            const float x = radius * sin(deg2rad(azimuth)) * sin(deg2rad(elevation));
            const float y = radius * cos(deg2rad(elevation));
            const float z = radius * cos(deg2rad(azimuth)) * sin(deg2rad(elevation));
            eye = vec3(x, y, z);
        } else {
            fpsAzimuth   += dx * mouseLookSensitivity;
            fpsElevation -= dy * mouseLookSensitivity;

            if (fpsAzimuth > 360.0f) fpsAzimuth -= 360.0f;
            if (fpsAzimuth < 0.0f)   fpsAzimuth += 360.0f;
            if (fpsElevation > 89.0f)  fpsElevation = 89.0f;
            if (fpsElevation < -89.0f) fpsElevation = -89.0f;
            // center is recomputed from these angles in display()
        }

        return;   // skip gizmo logic while looking
    }

    SceneNode* selected = sceneManager.get_selected();
    if (activeGizmo == GIZMO_NONE || !selected) return;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        activeGizmo = GIZMO_NONE;
        return;
    }

    const vec2 delta = vec2(mx, my) - dragStartMouse;

    auto axis = vec3(0.0f, 0.0f, 0.0);
    if (activeGizmo == GIZMO_X) axis = vec3(1, 0, 0);
    else if (activeGizmo == GIZMO_Y) axis = vec3(0, 1, 0);
    else if (activeGizmo == GIZMO_Z) axis = vec3(0, 0, 1);

    // column-vector: viewAxis = camera_matrix * axis (w = 0)
    vec3 viewAxis;
    viewAxis[0] = cameraMatrix[0][0]*axis[0] + cameraMatrix[1][0]*axis[1] + cameraMatrix[2][0]*axis[2];
    viewAxis[1] = cameraMatrix[0][1]*axis[0] + cameraMatrix[1][1]*axis[1] + cameraMatrix[2][1]*axis[2];
    viewAxis[2] = cameraMatrix[0][2]*axis[0] + cameraMatrix[1][2]*axis[1] + cameraMatrix[2][2]*axis[2];

    vec2 screenDir(viewAxis[0], viewAxis[1]);
    float len = length(screenDir);
    if (len < 0.001f) len = 1.0f;
    screenDir = screenDir * (1.0f / len);

    // distance-based sensitivity so it feels the same in both modes
    const float dist = length(eye - selected->position);
    const float sensitivity = 0.0015f * dist;

    const float amount = dot(delta, screenDir) * sensitivity;

    selected->position = dragStartPos + axis * amount;
    update_fields();
}

inline void scroll_callback(GLFWwindow* window, const double xOffset, const double yOffset) {
    if (cameraMode != ORTHO) return;

    // yOffset > 0 means scroll up (zoom in), < 0 means scroll down (zoom out)
    const float zoom = static_cast<float>(yOffset) * dr * 4.0f;   // scale factor, tune as needed

    radius -= zoom;
    if (radius < minRadius)
        radius = minRadius;

    // recompute eye from spherical coords (same as handle_key_ortho)
    const float x = radius * sin(deg2rad(azimuth)) * sin(deg2rad(elevation));
    const float y = radius * cos(deg2rad(elevation));
    const float z = radius * cos(deg2rad(azimuth)) * sin(deg2rad(elevation));
    eye = vec3(x, y, z);
}




//-----------------------------------------HELPERS----------------------------------------------
//----------------------------------------------------------------------------------------------

//update on window resize
inline void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
    ww = width;
    hh = height;
    create_outline_fbo(ww, hh);
}

//ensure cursor matching size of window and proper scaling
inline void cursor_to_framebuffer(GLFWwindow* window, const double xPos, const double yPos, float& mx, float& my) {
    int win_w, win_h;
    glfwGetWindowSize(window, &win_w, &win_h);
    if (win_w <= 0 || win_h <= 0) {
        mx = static_cast<float>(xPos);
        my = static_cast<float>(hh) - static_cast<float>(yPos);
        return;
    }
    const float xscale = static_cast<float>(ww) / static_cast<float>(win_w);
    const float yscale = static_cast<float>(hh) / static_cast<float>(win_h);
    mx = static_cast<float>(xPos) * xscale;
    my = static_cast<float>(hh) - static_cast<float>(yPos) * yscale;
}

inline void handle_key_ortho(const int key, int action) {
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
        if (radius < minRadius)
        {
            radius = minRadius;
        }
    }

    const GLfloat x = radius * sin(deg2rad(azimuth)) * sin(deg2rad(elevation));
    const GLfloat y = radius * cos(deg2rad(elevation));
    const GLfloat z = radius * cos(deg2rad(azimuth)) * sin(deg2rad(elevation));
    eye = vec3(x,y,z);
}

inline void handle_key_perspective(const int key, int action) {
    // Arrow keys – look only
    if (key == GLFW_KEY_LEFT) {
        fpsAzimuth -= fpsTurn;
        if (fpsAzimuth > 360.0f) fpsAzimuth -= 360.0f;
    } else if (key == GLFW_KEY_RIGHT) {
        fpsAzimuth += fpsTurn;
        if (fpsAzimuth < 0.0f) fpsAzimuth += 360.0f;
    }

    if (key == GLFW_KEY_UP) {
        fpsElevation += fpsTurn;
        if (fpsElevation > 89.0f) fpsElevation = 89.0f;
    } else if (key == GLFW_KEY_DOWN) {
        fpsElevation -= fpsTurn;
        if (fpsElevation < -89.0f) fpsElevation = -89.0f;
    }

    // W/S – move forward/backward
    // A/D – strafe left/right
    if (key == GLFW_KEY_W || key == GLFW_KEY_S ||
    key == GLFW_KEY_A || key == GLFW_KEY_D||
    key == GLFW_KEY_SPACE || key == GLFW_KEY_LEFT_CONTROL) {

        const float yaw   = deg2rad(fpsAzimuth);
        const float pitch = deg2rad(fpsElevation);
        const vec3 forward = normalize(vec3(
            cos(yaw) * cos(pitch),
            sin(pitch),
            sin(yaw) * cos(pitch)
        ));
        const vec3 right = normalize(cross(forward, up));

        if (key == GLFW_KEY_W) eye += forward * fpsStep;
        if (key == GLFW_KEY_S) eye -= forward * fpsStep;
        if (key == GLFW_KEY_A) eye -= right   * fpsStep;
        if (key == GLFW_KEY_D) eye += right   * fpsStep;
        if (key == GLFW_KEY_SPACE)          eye[1] += fpsStep;
        if (key == GLFW_KEY_LEFT_CONTROL)   eye[1] -= fpsStep;
        }
}


#endif //CALLBACKS_H