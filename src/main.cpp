#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <iostream>
#include <string>
#include <memory>

#include "model.h"

#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader.h"

#include "SceneObjects/pyramid.h"
using namespace Settings::Window;

void window_initialise(GLFWwindow *&window) {
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
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPos(window, WIDTH, HEIGHT);
}

int main() {
    GLFWwindow *window = nullptr;
    window_initialise(window);
    // -------------------------------------------------------------------------
    std::shared_ptr<ShaderProgram> texture_shader = std::make_shared<ShaderProgram>("shaders/temp.vertex", "shaders/temp.fragment");
    // -------------------------------------------------------------------------
    Model model("res/models/skull/12140_Skull_v3_L2.obj", texture_shader);
    model.scale({0.1, 0.1, 0.1});
    // model.rotate(90, {1, 0, 0});
    // -------------------------------------------------------------------------
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    Piramid piramid(color);

    Model piramid("path");

    // -------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowUserPointer(window, &controller);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ---------------------------------------------------------------------

        model.draw(camera);


        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
