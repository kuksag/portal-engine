#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

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

    auto a = scene.add_primitive<Cylinder>({0, 1.2, 0}, {0.5, 0, 0});
    auto b = scene.add_primitive<Cube>({0, 3, 0}, {0, 0, 1});
    a->translate({-5, 2, 0});
    a->rotate(3.1415 / 4, {1, 0, 0});
    a->scale({0.2, 0.2, 0.2});

    auto c = scene.add_primitive<Torus>({3, 0, 0}, {0, 0, 0});
    
    b->link_to(a); //unlink <=> link to nullptr
    c->link_to(b);

    scene.add_light({5, 25, 5}, {1, 1, 1}, 0.6f, false);
    scene.add_light({5, 25, 5}, {0.7, 0.8, 0.9}, 0.8f, true);


    // -------------------------------------------------------------------------
    do {
        scene.update();
        scene.draw();
        glfwPollEvents();
    } while (window.is_open());

    glfwTerminate();
}
