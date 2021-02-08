#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader-loader.h"
#include "camera.h"

int main() {
    GLFWwindow *window;
    int WIDTH = 1024, HEIGHT = 768;

    // initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                   GL_TRUE);    // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Hello world", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to open GLFW window\n";
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



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

    Camera camera;
    double x_mouse = 0;
    double y_mouse = 0;
    double last_time = 0;
    do {
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_id);

        glm::mat4 model_matrix = glm::mat4(1.0f);
        glfwGetCursorPos(window, &x_mouse, &y_mouse);

        auto time_delta = static_cast<float>(glfwGetTime() - last_time);
        last_time = glfwGetTime();
        auto x_delta = WIDTH / 2.0 - x_mouse;
        auto y_delta = HEIGHT / 2.0 - y_mouse;

        glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);

        // TODO: controls.cpp
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            camera.move_forward(time_delta);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera.move_backward(time_delta);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            camera.move_right(time_delta);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            camera.move_left(time_delta);
        }
        camera.process_mouse_move(x_delta, y_delta);
        glm::mat4 MVP = camera.get_projection_matrix() * camera.get_view_matrix() *
                        model_matrix;

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}
