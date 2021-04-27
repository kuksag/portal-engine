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

	Scene scene(window, camera);
	scene.set_bg_color({0.3, 0.3, 0.6});

	scene.add_primitive<Torus>({1, 0, 0}, {1, 1, 0});
	std::shared_ptr<LightSource> l = scene.add_light({1, 10, 1}, {1, 1, 1});
	std::shared_ptr<Primitive> p = scene.add_primitive<Plane>({0, -1, 0}, {0.34, 0.28, 0.67});
	p->scale({100, 1, 100});


    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        scene.draw();        

        glfwPollEvents();
    } while (window.is_open());

    glfwTerminate();
}
