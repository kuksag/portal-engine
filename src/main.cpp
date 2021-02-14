#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader-loader.h"
using namespace Settings::Window;

void window_initialise(GLFWwindow *&window) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        getchar();
        assert(false);
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        assert(false);
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        assert(false);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, 1);

    glfwSetCursorPos(window, WIDTH, HEIGHT);
}

int main() {
    GLFWwindow *window = nullptr;
    window_initialise(window);
    // -------------------------------------------------------------------------
    GLuint vertex_array_id;
    glGenVertexArrays(1, &vertex_array_id);
    glBindVertexArray(vertex_array_id);

    static const GLfloat vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
                 vertex_buffer_data, GL_STATIC_DRAW);

    GLuint program_id = load_shader("res/shaders/temp.vertexshader",
                                    "res/shaders/temp.fragmentshader");

    GLuint MatrixID = glGetUniformLocation(program_id, "MVP");
    // -------------------------------------------------------------------------
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();
        glfwSetWindowUserPointer(window, &controller);
        glfwSetScrollCallback(window, scroll_callback);

        // ---------------------------------------------------------------------

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_id);

        glm::mat4 model_matrix = glm::mat4(1.0f);
        glm::mat4 MVP = camera.get_projection_matrix() *
                        camera.get_view_matrix() * model_matrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(0,    // attribute 0. No particular reason for 0,
                                    // but must match the layout in the shader.
                              3,    // size
                              GL_FLOAT,    // type
                              GL_FALSE,    // normalized?
                              0,           // stride
                              nullptr      // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0,
                     3);    // 3 indices starting at 0 -> 1 triangle

        glDisableVertexAttribArray(0);

        // ---------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
