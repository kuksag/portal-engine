#include "window.h"
#include "settings.h"

#include <iostream>
#include <cassert>

using namespace Settings::Window;//TODO: move to class

Window::Window() {
	if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        getchar();
        assert(false);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        assert(false);
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, WIDTH, HEIGHT);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        assert(false);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPos(window, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);
}



GLFWwindow* Window::glfw_window() {
	return window;
}