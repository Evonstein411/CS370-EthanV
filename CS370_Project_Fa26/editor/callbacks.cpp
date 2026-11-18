#include "callbacks.h"
#include "editorcontroller.h"
#include "../common/GLFWutils.h"

Callbacks* Callbacks::instance = nullptr;

void Callbacks::register_all(GLFWwindow* window, EditorController* ctrl) {
    controller = ctrl;
    instance = this;

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCharCallback(window, char_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Callbacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (instance && instance->controller)
        instance->controller->handle_key(key, action, mods);
}

void Callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if (instance && instance->controller)
        instance->controller->handle_mouse_button(button, action, x, y);
}

void Callbacks::cursor_callback(GLFWwindow* window, double x, double y) {
    if (instance && instance->controller)
        instance->controller->handle_cursor(x, y);
}

void Callbacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (instance && instance->controller)
        instance->controller->handle_scroll(xoffset, yoffset);
}

void Callbacks::char_callback(GLFWwindow* window, unsigned int codepoint) {
    if (instance && instance->controller)
        instance->controller->handle_char(codepoint);
}

void Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (instance && instance->controller)
        instance->controller->handle_resize(width, height);
}