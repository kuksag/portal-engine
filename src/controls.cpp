#include "controls.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "settings.h"
using namespace Settings::Controls;
using namespace Settings::Window;

Controller::Controller(Camera& camera_, Window& window_)
    : camera(camera_),
      window(window_),
      last_time_point(),
      is_fullscreen(false) {
    //window_initialise();
        // setup functions-callback
    glfwSetWindowUserPointer(window.glfw_window(), this);
    glfwSetScrollCallback(window.glfw_window(), glfw_scroll_callback);
    glfwSetFramebufferSizeCallback(window.glfw_window(), glfw_framebuffer_size_callback);
    glfwSetWindowFocusCallback(window.glfw_window(), glfw_focus_callback);
    update_time();
}

void Controller::scroll_callback(double y_delta) {
    camera.process_mouse_scroll(static_cast<float>(y_delta));
}

void Controller::cursor_position_callback_without_changes() {
    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;
    int width, height;
    glfwGetWindowSize(window.glfw_window(), &width, &height);

    double x_mouse = 0;
    double y_mouse = 0;
    glfwGetCursorPos(window.glfw_window(), &x_mouse, &y_mouse);

    auto x_delta = static_cast<float>(width / 2.0 - x_mouse);
    auto y_delta = static_cast<float>(height / 2.0 - y_mouse);

    camera.process_mouse_move(static_cast<float>(x_delta),
                              static_cast<float>(y_delta), time_delta);
}

void Controller::cursor_position_callback() {
    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;
    int width, height;
    glfwGetWindowSize(window.glfw_window(), &width, &height);

    double x_mouse = 0;
    double y_mouse = 0;
    glfwGetCursorPos(window.glfw_window(), &x_mouse, &y_mouse);

    auto x_delta = static_cast<float>(width / 2.0 - x_mouse);
    auto y_delta = static_cast<float>(height / 2.0 - y_mouse);

    camera.process_mouse_move(static_cast<float>(x_delta),
                               static_cast<float>(y_delta), time_delta);

    glfwSetCursorPos(window.glfw_window(), width / 2.0, height / 2.0);
}

void Controller::key_callback() {
    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;

    // move camera forward
    for (auto &key : KEY_FORWARD) {
        if (glfwGetKey(window.glfw_window(), key) == GLFW_PRESS) {
            camera.move_forward(time_delta);
        }
    }

    // move camera rightward
    for (auto &key : KEY_RIGHT) {
        if (glfwGetKey(window.glfw_window(), key) == GLFW_PRESS) {
            camera.move_right(time_delta);
        }
    }

    // move camera backward
    for (auto &key : KEY_BACKWARD) {
        if (glfwGetKey(window.glfw_window(), key) == GLFW_PRESS) {
            camera.move_backward(time_delta);
        }
    }

    // move camera leftward
    for (auto &key : KEY_LEFT) {
        if (glfwGetKey(window.glfw_window(), key) == GLFW_PRESS) {
            camera.move_left(time_delta);
        }
    }

    if (glfwGetKey(window.glfw_window(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.move_up(time_delta);
    }

    if (glfwGetKey(window.glfw_window(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        camera.move_down(time_delta);
    }

    for (auto &key : KEY_FULLSCREEN) {
        if (glfwGetKey(window.glfw_window(), key) == GLFW_PRESS) {
            toggle_fullscreen(true);
        }
    }

    for (auto &key : KEY_WINDOWED) {
        if (glfwGetKey(window.glfw_window(), key) == GLFW_PRESS) {
            toggle_fullscreen(false);
        }
    }
}



void Controller::update_time() {
    last_time_point = static_cast<float>(glfwGetTime());
}

void Controller::toggle_fullscreen(bool flag) {
    static int pos_x_last = 0;
    static int pos_y_last = 0;
    static int width_last_size = Settings::Window::WIDTH;
    static int height_last_size = Settings::Window::HEIGHT;

    if (flag == is_fullscreen) return;

    if (flag) {
        glfwGetWindowPos(window.glfw_window(), &pos_x_last, &pos_y_last);
        glfwGetWindowSize(window.glfw_window(), &width_last_size, &height_last_size);

        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        glfwSetWindowMonitor(window.glfw_window(), glfwGetPrimaryMonitor(), 0, 0, mode->width,
                             mode->height, mode->refreshRate);
        glfw_framebuffer_size_callback(window.glfw_window(), mode->width, mode->height);
        is_fullscreen = true;
    } else {
        glfwSetWindowMonitor(window.glfw_window(), nullptr, pos_x_last, pos_y_last,
                             width_last_size, height_last_size, 0);
        glfw_framebuffer_size_callback(window.glfw_window(), width_last_size,
                                       height_last_size);
        is_fullscreen = false;
    }
}

void glfw_scroll_callback(GLFWwindow *window, double x_delta, double y_delta) {
    auto *controller =
        reinterpret_cast<Controller *>(glfwGetWindowUserPointer(window));
    if (controller) {
        controller->scroll_callback(y_delta);
    } else {
        assert(false);
    }
}

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_focus_callback(GLFWwindow *window, int focused) {
    if (focused == GLFW_TRUE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

glm::vec3 Controller::get_position() const {
    return camera.get_position();
}

glm::vec3 Controller::get_position_after_move() {
    auto old_version_of_camera = camera;
    cursor_position_callback_without_changes();
    key_callback();
    glm::vec3 position = get_position();
    camera = old_version_of_camera;
    return position;
}
bool Controller::is_enter_pressed() {
    return glfwGetKey(window.glfw_window(), GLFW_KEY_ENTER) == GLFW_PRESS;
}
float Controller::get_time_delta() {
    return glfwGetTime() - last_time_point;
}
