#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "camera.h"
#include "controls.h"
#include "light_source.h"
#include "primitives.h"

using namespace Settings::Window;

void render_scene(const std::vector<Drawable*> &drawables, const Camera &camera) {
    for (auto &primitive : drawables) {
        primitive->draw(camera);
    }
}

void depth_test_render_scene(const std::vector<Drawable*> &drawables, const Camera &camera, std::shared_ptr<ShaderProgram> depth_shader) {
    for (auto &primitive : drawables) {
        primitive->depth_test_draw(camera, depth_shader);
    }
}

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
    std::vector<LightSource> light_sources{LightSource(glm::vec3(10.0f, 10.0f, 10.0f),
                             glm::vec3(1.0f, 0.5f, 1.0f))};
    // -------------------------------------------------------------------------
    std::shared_ptr<ShaderProgram> lighted_shader(new ShaderProgram("shaders/light.vertex", "shaders/light.fragment"));
    Model skull("res/models/skull/12140_Skull_v3_L2.obj", lighted_shader);
    skull.translate(glm::vec3(0.0f, 5.0f, 0.0f));
    skull.scale(glm::vec3(0.15f, 0.15f, 0.15f));
    // -------------------------------------------------------------------------
    std::vector<Drawable *> drawables = {
        new Cube({0, 0, 0}, {0.5, 0.5, 0}),
        new Sphere({0, 2, 0}, {0.2, 1, 0.5}),
        new Plane({0, -6, 0}, {0.3, 0.1, 0.8}),
        new Cylinder({4, 0, 0}, {0.4, 0.2, 0.2}),
        new Torus({2, 2, 2}, {0.2, 0.5, 0.5}),
        new Cone({0, -4, 0}, {0.4, 1, 1}),
        &skull};
    // -------------------------------------------------------------------------
    for (auto &primitive : drawables) {
        primitive->set_light_sources(&light_sources);
    }
    // -------------------------------------------------------------------------
    //Big Plane
        drawables[2]->scale(glm::vec3(100.0f, 100.0f, 100.0f));
    // -------------------------------------------------------------------------
        std::shared_ptr<ShaderProgram> depth_shader(new ShaderProgram("shaders/depth.vertex", "shaders/depth.fragment"));
    // -------------------------------------------------------------------------
    const int SHADOW_WIDTH = 3000, SHADOW_HEIGHT = 3000;
    GLuint depth_map_fbo;
    glGenFramebuffers(1, &depth_map_fbo);
    GLuint depth_map;
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = {0.3f, 0.3f, 0.3f, 0.3f};
    glTexParameterfv(GL_TEXTURE_2D, GL_CONVOLUTION_BORDER_COLOR, border_color);

    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // -------------------------------------------------------------------------
    light_sources[0].set_depth_map(depth_map);
    // -------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        depth_test_render_scene(drawables, light_sources[0].get_camera(), depth_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // ---------------------------------------------------------------------
        render_scene(drawables, camera);

        //depth_test_render_scene(drawables, light_sources[0].get_camera(), depth_shader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
