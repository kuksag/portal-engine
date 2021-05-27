#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "camera.h"
#include "controls.h"
#include "portal.h"
#include "primitives.h"
#include "puzzle.h"
#include "light_source.h"
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
    cube->translate({-2, 1.2, 0});
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
