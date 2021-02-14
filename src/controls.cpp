#include "controls.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "settings.h"
using namespace Settings::Controls;

Controller::Controller(Camera *camera_, GLFWwindow *window_)
    : camera(camera_),
      window(window_),
      last_time_point(),
      is_fullscreen(false) {
    update_time();
}

void Controller::scroll_callback(double y_delta) {
    camera->process_mouse_scroll(static_cast<float>(y_delta));
}

void Controller::cursor_position_callback() {
    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double x_mouse = 0;
    double y_mouse = 0;
    glfwGetCursorPos(window, &x_mouse, &y_mouse);

    auto x_delta = static_cast<float>(width / 2.0 - x_mouse);
    auto y_delta = static_cast<float>(height / 2.0 - y_mouse);

    camera->process_mouse_move(static_cast<float>(x_delta),
                               static_cast<float>(y_delta), time_delta);

    glfwSetCursorPos(window, width / 2.0, height / 2.0);
}

void Controller::key_callback() {
    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;

    // move camera forward
    for (auto &key : KEY_FORWARD) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera->move_forward(time_delta);
        }
    }

    // move camera rightward
    for (auto &key : KEY_RIGHT) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera->move_right(time_delta);
        }
    }

    // move camera backward
    for (auto &key : KEY_BACKWARD) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera->move_backward(time_delta);
        }
    }

    // move camera leftward
    for (auto &key : KEY_LEFT) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            camera->move_left(time_delta);
        }
    }

    for (auto &key : KEY_FULLSCREEN) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            toggle_fullscreen();
        }
    }
}

void Controller::update_time() {
    last_time_point = static_cast<float>(glfwGetTime());
}

void Controller::toggle_fullscreen() {
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (is_fullscreen) {
        glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0,
                             Settings::Window::WIDTH, Settings::Window::HEIGHT,
                             mode->refreshRate);
        glViewport(0, 0, Settings::Window::WIDTH, Settings::Window::HEIGHT);
    } else {
        glfwSetWindowMonitor(window, nullptr, 0, 0,
                             Settings::Window::FULL_WIDTH,
                             Settings::Window::FULL_HEIGHT, mode->refreshRate);
        glViewport(0, 0, Settings::Window::FULL_WIDTH,
                   Settings::Window::FULL_HEIGHT);
    }
    is_fullscreen = !is_fullscreen;
}

void scroll_callback(GLFWwindow *window, double x_delta, double y_delta) {
    auto *controller =
        reinterpret_cast<Controller *>(glfwGetWindowUserPointer(window));
    if (controller) {
        controller->scroll_callback(y_delta);
    } else {
        assert(false);
    }
}