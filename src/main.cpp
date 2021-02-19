#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <iostream>
#include <string>

#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader.h"
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
    glViewport(0, 0, WIDTH, HEIGHT);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        assert(false);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPos(window, WIDTH, HEIGHT);
}

int main() {
    GLFWwindow *window = nullptr;
    window_initialise(window);
    // -------------------------------------------------------------------------

    ShaderProgram temp_shader("temp.vertex", "temp.fragment");

    static const GLfloat vertex_buffer_data[] = {
        /// Позиуии вершин     Позици координат
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f,
        1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    };

    GLuint indices[] = {
        0,
        1,
        2,
    };
    ///Задает треугольники (тройки вершин), которые мы рисуем.
    /// В случае одного не обязательно это использовать

    GLuint vertex_buffer, vertex_array_id, element_buffer;
    glGenVertexArrays(1, &vertex_array_id);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &element_buffer);

    glBindVertexArray(vertex_array_id);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
                 vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid *)(3 * sizeof(GLfloat)));
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
    unsigned char *image = SOIL_load_image("res/textures/container.jpeg",
                                           &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    //--------------------------------------------------------------------------
    // Освещаемый куб
    static const GLfloat light_cube_vertexes[] = {
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  -1.0f,
        0.5f,  -0.5f, -0.5f,    0.0f,  0.0f,  -1.0f,
        0.5f,  0.5f,  -0.5f,    0.0f,  0.0f,  -1.0f,
        0.5f,  0.5f,  -0.5f,    0.0f,  0.0f,  -1.0f,
        -0.5f, 0.5f,  -0.5f,    0.0f,  0.0f,  -1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f,  -1.0f,

        -0.5f, -0.5f, 0.5f,     0.0f,  0.0f,  1.0f,
        0.5f,  -0.5f, 0.5f,     0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
        -0.5f, 0.5f,  0.5f,     0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  0.5f,    -1.0f, 0.0f,  0.0f,
        -0.5f, 0.5f,  -0.5f,   -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, 0.5f,    -1.0f, 0.0f,  0.0f,
        -0.5f, 0.5f,  0.5f,    -1.0f, 0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  -0.5f,    1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, 0.5f,     1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, -0.5f,    0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,     0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,     0.0f,  -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,     0.0f,  -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  -1.0f, 0.0f,

        -0.5f, 0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  -0.5f,    0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
        -0.5f, 0.5f,  0.5f,     0.0f,  1.0f,  0.0f,
        -0.5f, 0.5f,  -0.5f,    0.0f,  1.0f,  0.0f
    };

    GLuint light_vertex_buffer, light_vertex_array_id;
    glGenVertexArrays(1, &light_vertex_array_id);
    glGenBuffers(1, &light_vertex_buffer);

    glBindVertexArray(light_vertex_array_id);

    glBindBuffer(GL_ARRAY_BUFFER, light_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_cube_vertexes),
                 light_cube_vertexes, GL_STATIC_DRAW);
    // vertex coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    ShaderProgram light_shader("light.vertex", "light.fragment");

    // -------------------------------------------------------------------------
    GLfloat fancy_triangle[] = {1.0f, 0.0f, 0.0,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                0.0,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0,  0.5f,
                                0.5f, 0.5f, 0.0f, 0.0f, 1.0,  0.0f, 0.0f, 1.0f};

    int fancy_indices[]{0, 2, 1, 0, 2, 3, 0, 1, 3, 1, 2, 3};

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fancy_indices), fancy_indices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fancy_triangle), fancy_triangle,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    ShaderProgram fancy_shader("fancy.vertex", "fancy.fragment");

    // -------------------------------------------------------------------------
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowUserPointer(window, &controller);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ---------------------------------------------------------------------

        glm::mat4 model_matrix = glm::mat4(1.0f);
        glm::mat4 MVP = camera.get_projection_matrix() *
                        camera.get_view_matrix() * model_matrix;

        // ---------------------------------------------------------------------
        glBindTexture(GL_TEXTURE_2D, texture);
        temp_shader.use();

        glUniformMatrix4fv(temp_shader.get_uniform_id("MVP"), 1, GL_FALSE,
                           &MVP[0][0]);
        glBindVertexArray(vertex_array_id);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        // ---------------------------------------------------------------------
        // pyramid pos, color, transformations etc.
        float time_value = glfwGetTime();
        glm::mat4 rotation_matrix(1.0f), translation_matrix(1.0f),
            scale_matrix(1.0f);

        rotation_matrix = glm::rotate(rotation_matrix, time_value,
                                      glm::vec3(6 * M_PI, M_PI, M_PI_2));
        float radius = 3.0f;
        glm::vec3 pivot = {3.0f, 3.0f, 3.0f};
        translation_matrix = glm::translate(
            translation_matrix,
            glm::vec3(cos(time_value / 2) * radius, sin(time_value / 2),
                      sin(time_value / 2) * radius) +
                pivot);

        glm::mat4 pyramid_mode_matrix =
            translation_matrix * rotation_matrix * scale_matrix;

        glm::mat4 current_center_m4 =
            pyramid_mode_matrix * glm::mat4(glm::vec4(1.0f), glm::vec4(1.0f),
                                            glm::vec4(1.0f), glm::vec4(1.0f));
        glm::vec3 current_center_v3 = {current_center_m4[0][0],
                                       current_center_m4[0][1],
                                       current_center_m4[0][2]};
        // ---------------------------------------------------------------------
        // pyrmaid camera
        fancy_shader.use();
        glm::mat4 MVP_PYRAMID = camera.get_projection_matrix() *
                                camera.get_view_matrix() * pyramid_mode_matrix;
        glUniformMatrix4fv(fancy_shader.get_uniform_id("MVP"), 1, GL_FALSE,
                           &MVP_PYRAMID[0][0]);
        // ---------------------------------------------------------------------
        // pyramid shape draw
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glm::vec3 current_fancy_color = {0.0f, 0.0f, 0.0f};
        glUniform3f(fancy_shader.get_uniform_id("fancy_color"),
                    current_fancy_color[0], current_fancy_color[1],
                    current_fancy_color[2]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // ---------------------------------------------------------------------
        // pyramid draw
        current_fancy_color = {sin(time_value), 0.2, cos(time_value)};
        glUniform3f(fancy_shader.get_uniform_id("fancy_color"),
                    current_fancy_color[0], current_fancy_color[1],
                    current_fancy_color[2]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        // ---------------------------------------------------------------------
        // "cube of shade" draw
        light_shader.use();
        glUniform3f(light_shader.get_uniform_id("trans"), 3.0f, 3.0f, 3.0f);
        glUniformMatrix4fv(light_shader.get_uniform_id("MVP"), 1, GL_FALSE,
                           &MVP[0][0]);
        glUniform3f(light_shader.get_uniform_id("light_color"),
                    current_fancy_color[0], current_fancy_color[1],
                    current_fancy_color[2]);
        glUniform3f(light_shader.get_uniform_id("light_pos"),
                    current_center_v3[0], current_center_v3[1],
                    current_center_v3[2]);
        glBindVertexArray(light_vertex_array_id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //----------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &vertex_array_id);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &element_buffer);

    glfwTerminate();
}
