#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <memory>

#include "model.h"
#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader.h"

using namespace Settings::Window;

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    std::shared_ptr<ShaderProgram> texture_shader = std::make_shared<ShaderProgram>("shaders/temp.vertex", "shaders/temp.fragment");
    // -------------------------------------------------------------------------
    Model model("res/models/skull/12140_Skull_v3_L2.obj", texture_shader);
    model.scale({0.1, 0.1, 0.1});
    // -------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
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
