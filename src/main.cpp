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
    scene.add_primitive();//TODO: prim type

    do {
        controller.cursor_position_callback(); //TODO: make one method
        controller.key_callback();//TODO: make one method
        controller.update_time();//TODO: make one method

        scene.draw();

        glfwPollEvents();
    } while (window.is_open());

    glfwTerminate();
}
