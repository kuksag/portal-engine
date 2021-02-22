#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "camera.h"
#include "controls.h"
#include "settings.h"
#include "shader.h"
#include "SceneObjects/triangle.h"

#include "SceneObjects/pyramid.h"
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
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPos(window, WIDTH, HEIGHT);
}

int main() {
    GLFWwindow *window = nullptr;
    window_initialise(window);
    // -------------------------------------------------------------------------
    ShaderProgram temp_shader("shaders/temp.vertex", "shaders/temp.fragment");

    Triangle textured_triangle;
    std::cout << glGetError() << std::endl;
    //textured_triangle.translate(glm::vec3(-0.5f, -0.5f, -0.5f));
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

    ShaderProgram light_shader("shaders/light.vertex", "shaders/light.fragment");
    // -------------------------------------------------------------------------
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glfwSetWindowUserPointer(window, &controller);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // -------------------------------------------------------------------------
    Pyramid rainbow_pyramid;
    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ---------------------------------------------------------------------

        glm::mat4 model_matrix = glm::mat4(1.0f);
        glm::mat4 MVP = camera.get_projection_matrix() *
                        camera.get_view_matrix() * model_matrix;
        glm::mat4 projection_view = camera.get_projection_matrix() *
                                    camera.get_view_matrix();
        // ---------------------------------------------------------------------
        textured_triangle.draw(projection_view);
        // ---------------------------------------------------------------------
        // calculation for rainbow-pyramid
        float radius = 3.0f;
        glm::vec3 pivot = {3.0f, 3.0f, 3.0f};
        float time_value = glfwGetTime();
        auto color_per_frame = glm::vec3(sin(time_value), 0.2, cos(time_value));
        auto rotation_per_frame = glm::vec3(6 * M_PI, M_PI, M_PI_2);
        auto translate_per_frame = pivot + glm::vec3(cos(time_value / 2) * radius,
                                                     sin(time_value / 2),
                                                     sin(time_value / 2) * radius);
        // ---------------------------------------------------------------------
        // pyramid draw
        rainbow_pyramid.supply_shader(color_per_frame);
        rainbow_pyramid.set_rotation_matrix(glm::rotate(glm::mat4(1.0f),
                                                        time_value,
                                                        rotation_per_frame));
        rainbow_pyramid.set_translation_matrix(
            glm::translate(glm::mat4(1.0f), translate_per_frame));
        rainbow_pyramid.draw(projection_view);
        rainbow_pyramid.draw_shape(projection_view);
        // ---------------------------------------------------------------------
        // "cube of shade" draw
        light_shader.use();
        glUniform3f(light_shader.get_uniform_id("trans"), 3.0f, 3.0f, 3.0f);
        glUniformMatrix4fv(light_shader.get_uniform_id("MVP"), 1, GL_FALSE,
                           &MVP[0][0]);
        glUniform3f(light_shader.get_uniform_id("light_color"),
                    color_per_frame[0], color_per_frame[1], color_per_frame[2]);
        auto center = rainbow_pyramid.get_center();
        glUniform3f(light_shader.get_uniform_id("light_pos"),
                    center[0], center[1], center[2]);
        glBindVertexArray(light_vertex_array_id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        // ---------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
