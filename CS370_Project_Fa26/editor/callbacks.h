#ifndef CALLBACKS_H
#define CALLBACKS_H



struct GLFWwindow;
class EditorController;

class Callbacks {
public:
    void register_all(GLFWwindow* window, EditorController* controller);

private:
    EditorController* controller = nullptr;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double x, double y);
    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
    static void char_callback(GLFWwindow* window, unsigned int codepoint);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    static Callbacks* instance;
};

#endif //CALLBACKS_H