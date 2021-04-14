#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "camera.h"
#include "controls.h"
#include "window.h"
#include "scene.h"

int main() {
    Window window;
    Camera camera;
    Controller controller(camera, window);//TODO: camera from scene
                                         //TODO: swap(camera, window)

    Scene scene(window, camera); //TODO: own camera
    std::shared_ptr<Primitive> cube = scene.add_cube({10, 0, 0});
    scene.add_cube({10, 10, 0}, {255, 0, 0});
    scene.add_cube({10, 0, 4}, {0, 255, 0});


    do {
        controller.cursor_position_callback(); //TODO: make one method
        controller.key_callback();//TODO: make one method
        controller.update_time();//TODO: make one method

        scene.draw();

        glfwPollEvents();
    } while (window.is_open());

    glfwTerminate();
}
