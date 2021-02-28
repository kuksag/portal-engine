#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "cube.h"
#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader.h"
#include "light_source.h"
#include <vector>

using namespace Settings::Window;

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    std::shared_ptr<ShaderProgram> texture_shader = std::make_shared<ShaderProgram>("shaders/light.vertex", "shaders/light.fragment");
    // -------------------------------------------------------------------------
    Model model("res/models/skull/12140_Skull_v3_L2.obj", texture_shader);
    model.scale({0.1, 0.1, 0.1});
    // -------------------------------------------------------------------------
    Model model1("res/models/skull/12140_Skull_v3_L2.obj", texture_shader);
    model1.scale({0.1, 0.1, 0.1});
    model1.rotate(acos(-1.0f) / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    model1.translate(glm::vec3(-5.0f, -5.0f, -5.0f));
    // -------------------------------------------------------------------------
    Model flat("res/models/flat/CobbleStones.obj", texture_shader);
    flat.translate(glm::vec3(0.0f, -20.0f, 0.0f));
    // -------------------------------------------------------------------------
    LightSource light_source(glm::vec3(15.0f, 15.0f, 15.0f),
                             glm::vec3(0.5f, 1.0f, 0.5f));
    // -------------------------------------------------------------------------
    Cube cube(light_source.get_pos(), light_source.get_color());
    glEnable(GL_DEPTH_TEST);
    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ---------------------------------------------------------------------


        model.draw(camera, std::vector{light_source});
        model1.draw(camera, std::vector{light_source});
        flat.draw(camera, std::vector{light_source});
        cube.draw(camera, {});
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
