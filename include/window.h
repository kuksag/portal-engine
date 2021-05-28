#ifndef PORTAL_ENGINE_WINDOW_H
#define PORTAL_ENGINE_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

class Window {
private:
    GLFWwindow* window = nullptr;

public:
    Window();

    GLFWwindow* glfw_window();

    bool is_open();
};

#endif    // PORTAL_ENGINE_WINDOW_H