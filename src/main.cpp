#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "shader-loader.h"
#include "camera.h"
#include <SOIL/SOIL.h>

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

    GLuint program_id = load_shader("res/shaders/temp.vertexshader",
                                    "res/shaders/temp.fragmentshader");

    static const GLfloat vertex_buffer_data[] = {
        /// Позиуии вершин     Позици координат
         -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
          1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
          0.0f,  1.0f, 0.0f,   0.5f, 1.0f,
    };

    GLuint indices[] = {
        0, 1, 2,
    };
    ///Задает треугольники (тройки вершин), которые мы рисуем.
    /// В случае дного не обязательно это использовать

    GLuint vertex_buffer, vertex_array_id, element_buffer;
    glGenVertexArrays(1, &vertex_array_id);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_buffer);

    glBindVertexArray(vertex_array_id);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    /// Раньше подобный код с glVertexAttribPointer был в теле цикла
    /// С помощью vertex_array можно избавиться от копипаста
    /// И это кажется более правильный вариант с точки зрения OpenGL


    ///Загрузка текстур
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image("res/textures/container.jpeg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.



    GLuint MatrixID = glGetUniformLocation(program_id, "MVP");

    Camera camera;
    double x_mouse = 0;
    double y_mouse = 0;
    double last_time = 0;
    do {
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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

        glBindTexture(GL_TEXTURE_2D, texture);
        glUseProgram(program_id);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glBindVertexArray(vertex_array_id);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &vertex_array_id);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &element_buffer);

    glfwTerminate();

    return 0;
}
