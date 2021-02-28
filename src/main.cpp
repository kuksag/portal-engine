#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "primitives.h"
#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader.h"
#include "light_source.h"
#include <vector>

using namespace Settings::Window;

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    std::vector<Drawable*> primitives = {new Cube({0, 0, 0}, {0.5, 0.5, 0}), new Sphere({0, 4, 0}, {0.2, 1, 0.5}),
                                         new Plane({0, 0, 4}, {0.3, 0.1, 0.8}), new Cylinder({4, 0, 0}, {0.4, 0.2, 0.2}),
                                         new Torus({0, 0, -4}, {0.2, 0.5, 0.5}), new Cone({0, -4, 0}, {0.4, 1, 1})};
    // -------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ---------------------------------------------------------------------

        for (int i=0; i < primitives.size(); ++i)
            primitives[i]->draw(camera, {});

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
