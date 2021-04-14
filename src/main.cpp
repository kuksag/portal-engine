#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "camera.h"
#include "controls.h"
#include "light_source.h"
#include "portal.h"
#include "primitives.h"
#include "puzzle.h"

using namespace Settings::Window;

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    std::vector<LightSource> light_sources{
        LightSource(glm::vec3(10.0f, 10.0f, 10.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f)),
        LightSource(glm::vec3(-10.0f, 10.0f, -10.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f))};
    // -------------------------------------------------------------------------
    std::vector<Portal *> portals;
    std::vector<Drawable *> objects;
    // -------------------------------------------------------------------------
    JokersTrap JS;
    for (std::size_t i = 0; i < JS.EDGE_NUMBER; i++) {
        objects.push_back(JS.patterns[i].centroid);
        for (std::size_t j = 0; j < JS.EDGE_NUMBER; j++) {
            if (i != j)
                objects.push_back(&JS.patterns[i].portals[j]);
        }
    }
    for (auto &i : JS.base.portals) portals.push_back(&i);
    // -------------------------------------------------------------------------
    for (auto &primitive : objects)
        primitive->set_light_sources(&light_sources);
    // -------------------------------------------------------------------------
    glEnable(GL_CULL_FACE);
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_COLOR_BUFFER_BIT);
        // ---------------------------------------------------------------------
        render_scene(camera, objects, portals, 0);
        // ---------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
