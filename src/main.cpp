#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
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
        LightSource(glm::vec3(0.0f, 5.0f, -25.0f), glm::vec3(1.0f, 1.0f, 1.0f)),
        LightSource(glm::vec3(-10.0f, 4.0f, -15.0f),
                    glm::vec3(1.0f, 1.0f, 1.0f))};
    // -------------------------------------------------------------------------
    std::vector<Drawable *> objects;
    std::vector<Portal *> portals;
    Portal a;
    Portal b;
    a.translate({20, 0, 25});
    b.translate({-4, 0, 1});
    b.rotate(-M_PI_2, {0, 1, 0});

    a.set_destination(&b);
    b.set_destination(&a);

    portals.push_back(&a);
    portals.push_back(&b);
    // -------------------------------------------------------------------------
    Sphere center;
    center.set_color({1.0, 0.0, 0.0});
    center.scale(glm::vec3(0.1));

    Plane floor;
    floor.translate({0.0, -5.0, 0.0});
    floor.scale({100.0, 1.0, 100.0});

    Sphere sphere;
    sphere.set_color({0.0, 1.0, 0.0});
    sphere.translate({20.0, 0.0, 20.0});

    std::shared_ptr<ShaderProgram> lighted_shader(
        new ShaderProgram("shaders/light.vertex", "shaders/light.fragment"));
    Model skull("res/models/skull/12140_Skull_v3_L2.obj", lighted_shader);
    skull.translate({5.0, 0.0, 0.0});
    skull.scale(glm::vec3(0.1));
    skull.rotate(-M_PI_2, {1.0, 0.0, 0.0});
    skull.rotate(-M_PI_4, {0.0, 0.0, 1.0});

    objects.push_back(&center);
    objects.push_back(&floor);
    objects.push_back(&sphere);
    objects.push_back(&skull);

    // -------------------------------------------------------------------------
    floor.set_light_sources(&light_sources);
    center.set_light_sources(&light_sources);
    sphere.set_light_sources(&light_sources);
    skull.set_light_sources(&light_sources);
    // -------------------------------------------------------------------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    // -------------------------------------------------------------------------
    do {
        glm::vec3 first_point = controller.get_position();
        glm::vec3 last_point = controller.get_position_after_move();

        for (Portal *portal :portals) {
            if (portal->crossed(first_point, last_point)) {
                camera = get_portal_destination_camera(camera, *portal);
                break;
            }
        }
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
