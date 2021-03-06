#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "camera.h"
#include "controls.h"
#include "light_source.h"
#include "portal.h"
#include "primitives.h"

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
    std::vector<Drawable *> objects = {new Cube({0, 0, 0}, {0.5, 0.5, 0}),
                                       new Sphere({0, 4, 0}, {0.2, 1, 0.5}),
                                       new Plane({3, 7, 8}, {0.3, 0.1, 0.8}),
                                       new Cylinder({4, 0, 0}, {0.4, 0.2, 0.2}),
                                       new Torus({0, 0, -4}, {0.2, 0.5, 0.5}),
                                       new Cone({0, -4, 0}, {0.4, 1, 1})};
    // -------------------------------------------------------------------------
    // Big Plane
    objects[2]->scale(glm::vec3(10.0f, 10.0f, 10.0f));
    // -------------------------------------------------------------------------
    for (auto &primitive : objects) {
        primitive->set_light_sources(&light_sources);
    }
    // -------------------------------------------------------------------------
    Portal portal_a, portal_b;
    const int SIZE = 3;
    portal_a.translate({-3, 1.5, 3.0});
    portal_a.rotate(-M_PI_4, {0.0, 1.0, 0.0});
    portal_a.scale({3 * SIZE, SIZE, 1});

    portal_b.translate({3, 1.5, -3.0});
    portal_b.rotate(-M_PI_4, {0.0, 1.0, 0.0});
    portal_b.rotate(M_PI - M_PI / 100, {0.0, 1.0, 0.0});
    portal_b.scale({3 * SIZE, SIZE, 1});

    portal_a.set_destination(&portal_b);
    portal_b.set_destination(&portal_a);

    std::vector<Portal *> portals = {&portal_a, &portal_b};
    // -------------------------------------------------------------------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT);
        // ---------------------------------------------------------------------
        render_scene(camera, objects, portals);
        // ---------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
