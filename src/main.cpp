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

    auto c = scene.add_primitive<Cylinder>({2, 0, -1}, {0.1, 0.7, 0.2});
    c->scale({0.3, 0.3, 0.3});

	std::shared_ptr<Portal> p1 = scene.add_portal({2, 0, 2});
	std::shared_ptr<Portal> p2 = scene.add_portal({-2, 0, -2});
	p1->set_destination(p2.get());
	p2->set_destination(p1.get());


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
