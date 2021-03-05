#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "camera.h"
#include "controls.h"
#include "light_source.h"
#include "primitives.h"
#include "portal.h"
#include <memory>

using namespace Settings::Window;

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
    std::vector<LightSource> light_sources{LightSource(glm::vec3(10.0f, 10.0f, 10.0f),
                             glm::vec3(1.0f, 1.0f, 1.0f)),
                                           LightSource(glm::vec3(-10.0f, 10.0f, -10.0f),
                                                       glm::vec3(1.0f, 0.0f, 0.0f))};
    // -------------------------------------------------------------------------
    std::shared_ptr<ShaderProgram> lighted_shader(new ShaderProgram("shaders/light.vertex", "shaders/light.fragment"));
    Model skull("res/models/skull/12140_Skull_v3_L2.obj", lighted_shader);
    skull.translate(glm::vec3(00.0f, 8.0f, 0.0f));
    skull.scale(glm::vec3(0.15f, 0.15f, 0.15f));
    // -------------------------------------------------------------------------
    std::vector<Drawable *> primitives = {
        new Cube({0, 0, 0}, {0.5, 0.5, 0}),
        new Sphere({0, 4, 0}, {0.2, 1, 0.5}),
        new Plane({3, 7, 8}, {0.3, 0.1, 0.8}),
        new Cylinder({4, 0, 0}, {0.4, 0.2, 0.2}),
        new Torus({0, 0, -4}, {0.2, 0.5, 0.5}),
        new Cone({0, -4, 0}, {0.4, 1, 1})};
    // -------------------------------------------------------------------------
    //Big Plane
        primitives[2]->scale(glm::vec3(10.0f, 10.0f, 10.0f));
    // -------------------------------------------------------------------------
    Portal portal_a, portal_b;
    portal_a.translate({2.5, 2.5, 2.5});

    portal_b.translate({5, 4, 4});
    portal_b.rotate(M_PI / 3, {1.0, 0.0, 0.0});
    portal_b.rotate(-M_PI / 3, {0.0, 1.0, 0.0});
    portal_b.rotate(M_PI / 3, {0.0, 0.0, 1.0});
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

        for (auto &primitive : primitives) {
            // if you want to set light source; by default {} is set
            primitive->set_light_sources(&light_sources);
            primitive->draw(camera);
        }
        skull.set_light_sources(&light_sources);
        skull.draw(camera);
        // ---------------------------------------------------------------------
        portal_a.draw(camera);
        portal_b.draw(camera);
        // ---------------------------------------------------------------------


        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
