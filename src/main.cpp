#include <GLFW/glfw3.h>

#include <iostream>

int main() {
    GLFWwindow *window;

    /* Initialise GLFW */
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        getchar();
        return -1;
    }

    /* Open a window and create its OpenGL context */
    window = glfwCreateWindow(1024, 768, "Hello world", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window\n";
        getchar();
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    do {
        /* Clear the screen */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();

    }    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
