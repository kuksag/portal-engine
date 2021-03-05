#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "camera.h"
#include "controls.h"
#include "window.h"

#include "primitives.h"

int main() {
    Window window;
    Camera camera;
    Controller controller(camera, window);

    Cube cube;


    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.draw(camera);

        glfwSwapBuffers(window.glfw_window());
        glfwPollEvents();
    } while (glfwGetKey(window.glfw_window(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window.glfw_window()) == 0);

    glfwTerminate();
}
