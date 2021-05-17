#ifndef PORTAL_ENGINE_CONTROLS_H
#define PORTAL_ENGINE_CONTROLS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "window.h"

struct Controller {
private:
    Camera& camera;
    Window& window;
    
    float last_time_point;
    bool is_fullscreen;
    void cursor_position_callback_without_changes();
public:
    explicit Controller(Camera& camera_, Window& window);

    void scroll_callback(double y_delta);
    void cursor_position_callback();
    void key_callback();

    void update_time();
    float delta_time();

    void toggle_fullscreen(bool flag);

    [[nodiscard]] glm::vec3 get_position() const;

    [[nodiscard]] glm::vec3 get_position_after_move();

    // TODO:
    //    void mouse_button_callback(int button, int action, int mods);
};

void glfw_scroll_callback(GLFWwindow *window, double x_delta, double y_delta);

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height);

void glfw_focus_callback(GLFWwindow* window, int focused);

#endif    // PORTAL_ENGINE_CONTROLS_H
