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
    std::vector<Drawable *> objects;
    std::vector<Portal *> portals;
    // -------------------------------------------------------------------------
    Sphere center;
    center.set_color({1.0, 0.0, 0.0});
    center.scale(glm::vec3(0.1));

    Plane floor;
    floor.translate({0.0, -5.0, 0.0});
    floor.scale({100.0, 1.0, 100.0});

    JokersTrap JT;
    JT.translate(glm::vec3(5.0));
    JT.scale(glm::vec3(2.0));
    JT.rotate(M_PI_4, {0.5, 0.2, 0.7});

    objects.push_back(&center);
    objects.push_back(&floor);

    for (auto &object : JT.objects) objects.push_back(object);
    for (auto &portal : JT.portals) portals.push_back(portal);
    // -------------------------------------------------------------------------
    JT.set_light_sources(&light_sources);
    floor.set_light_sources(&light_sources);
    center.set_light_sources(&light_sources);
    // -------------------------------------------------------------------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // -------------------------------------------------------------------------
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
        for (LightSource &ls : light_sources) {
            ls.gen_depth_map(objects);
        }
        glViewport(0, 0, WIDTH, HEIGHT);
        // ---------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
