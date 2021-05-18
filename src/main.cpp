#include <GLFW/glfw3.h>

#include "camera.h"
#include "controls.h"
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

    auto p1 = scene.add_portal({2, 1.2, 2});
    auto p2 = scene.add_portal({2, 1.2, -2});
    p1->set_destination(p2.get());
    p2->set_destination(p1.get());

    p2->rotate(3.14, {0, -1, 0});

    // p1->scale({0.2, 1, 1});
    // p1->rotate(3.14 / 4, {1, 1, 0});

    // p2->rotate(3.14 / 2, {0, 0, 1});
    // p1->rotate(3.14 / 2, {0, 0, 1});

    // auto p = scene.add_primitive<Cube>();
    // p->scale({0.3, 0.4, 0.3});

    auto c = scene.add_primitive<Torus>({2, 1.2, 0}, {0.3, 0, 0.3});
    auto c1 = scene.add_primitive<Cone>({2, 2.2, -2}, {0.2, 0.6, 0.4});
    c1->link_to(c.get());
    c->scale({0.3, 0.3, 0.3});
    c->rotate(-M_PI_2, {0.3, 0.5, 0.6});
    c->translate({1, 1, 1});
    scene.add_light({5, 25, 5}, {1, 1, 1}, 0.6f, false);
    scene.add_light({5, 25, 5}, {0.7, 0.8, 0.9}, 0.8f, true);

    // -------------------------------------------------------------------------
    do {
        scene.update();
        scene.draw();

        // p->set_translation_matrix(glm::mat4(1));
        // p->translate(camera.get_position() + glm::vec3{0, -0.6, 0});

        glfwPollEvents();
    } while (window.is_open());

    glfwTerminate();
}