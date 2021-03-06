#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "controls.h"
#include "light_source.h"
#include "portal.h"
#include "primitives.h"
#include "puzzle.h"
#include "scene.h"

using namespace Settings::Window;

int main() {
    Window window;
    Camera camera;
    Controller controller(camera, window);
    // -------------------------------------------------------------------------

    Scene scene(window, camera, controller);
    scene.set_bg_color({0.3, 0.3, 0.6});

    auto floor = scene.add_primitive<Plane>({0, 0, 0}, {0.5, 0.2, 0.3});
    floor->scale({10, 1, 10});

    auto wall1 = scene.add_primitive<Plane>({5, 1.2, 4}, {0.7, 0.8, 0.5});
    wall1->scale(glm::vec3(3.0f));
    wall1->rotate(M_PI_4, {0.4, 0.6, 1});
    auto wall2 = scene.add_primitive<Plane>({-5, 1.2, -2}, {0.7, 0.8, 0.5});
    wall2->scale(glm::vec3(3.0f));
    wall2->rotate(M_PI_2, {0, 0, 1});
    auto cube = scene.add_primitive<Cube>();
    cube->translate({1, 1.2, -2});
    cube->scale(glm::vec3(0.5));
    cube->set_color({0.4, 0.4, 0.4});

    auto sphere = scene.add_primitive<Sphere>();
    sphere->translate({4, 1.2, 5.7});
    sphere->scale(glm::vec3(0.5));
    sphere->set_color({0.69, 0.1488, 0.5});

    auto cone = scene.add_primitive<Cone>();
    cone->translate({-4, 1.2, 0});
    cone->scale(glm::vec3(0.5));
    cone->set_color({0.4, 0.4, 0.4});

    auto jt = scene.add_jokers_trap({3, 3, -5});
    jt->rotate(M_PI_2, {0.1, 0.4, 1});

    scene.add_light(glm::vec3(JokersTrap::MOVE_DISTANCE * 3.5f + 1), {1, 1, 1},  0.0f, false);
    scene.add_light(glm::vec3(JokersTrap::MOVE_DISTANCE * 7.5f + 1), {1, 1, 1},  0.0f, false);
    scene.add_light(glm::vec3(5.0f, 25.0f, 5.f), {0.3, 0.5, 0.1}, 1.5f, true);

    // -------------------------------------------------------------------------
    do {
        scene.update();
        scene.draw();

        glfwPollEvents();
    } while (window.is_open());

    glfwTerminate();
}
