#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <iostream>
#include <string>

#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader.h"

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
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
    /// В случае дного не обязательно это использовать

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
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
        0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
        0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
        0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
        -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
        1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
        0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
        0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
        0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
        0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

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
    glEnable(GL_DEPTH_TEST);
    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        // ---------------------------------------------------------------------

        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        float timeValue = glfwGetTime();
        fancy_shader.use();
        glm::vec3 current_fancy_color = {sin(timeValue), 0.2, cos(timeValue)};
        glUniform3f(fancy_shader.get_uniform_id("fancy_color"),
                    current_fancy_color[0], current_fancy_color[1],
                    current_fancy_color[2]);

        glm::mat4 translation_matrix(1.0f), scale_matrix(1.0f);
        scale_matrix =
            glm::scale(scale_matrix, glm::vec3(sin(glfwGetTime() / 8) * 2.0f,
                                               sin(glfwGetTime() / 8) * 2.0f,
                                               sin(glfwGetTime() / 8) * 2.0f));
        translation_matrix = glm::translate(
            translation_matrix, glm::vec3(sin(glfwGetTime() / 4) * 0.8f,
                                          cos(glfwGetTime() / 32) * 0.5f,
                                          cos(glfwGetTime() / 8) * 0.6f));

        glm::mat4 rotation_matrix(1.0f);
        rotation_matrix = glm::rotate(rotation_matrix, (float)glfwGetTime(),
                                      glm::vec3(0.4f, 0.8f, -0.1f));

        glm::mat4 current_center_m4 = scale_matrix * rotation_matrix *
                                      translation_matrix *
                                      glm::mat4({{1.0f, 1.0f, 1.0f, 1.0f},
                                                 {1.0f, 1.0f, 1.0f, 1.0f},
                                                 {1.0f, 1.0f, 1.0f, 1.0f},
                                                 {1.0f, 1.0f, 1.0f, 1.0f}});
        glm::vec3 current_center_v3 = {current_center_m4[0][0],
                                       current_center_m4[0][1],
                                       current_center_m4[0][2]};

        MVP = camera.get_projection_matrix() * camera.get_view_matrix() *
              scale_matrix * rotation_matrix * translation_matrix;
        glUniformMatrix4fv(fancy_shader.get_uniform_id("MVP"), 1, GL_FALSE,
                           &MVP[0][0]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        // ---------------------------------------------------------------------
        light_shader.use();
        glm::mat4 MVP_trans =
            camera.get_projection_matrix() * camera.get_view_matrix() *
            glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(light_shader.get_uniform_id("MVP"), 1, GL_FALSE,
                           &MVP_trans[0][0]);
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
        
        float distance = 0;
        for (int i = 0; i < 3; i++) {
            distance += (current_center_v3[i] - camera.get_position()[i]) * (current_center_v3[i] - camera.get_position()[i]);
        }
        std::cerr << sqrt(distance) << '\n';

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
