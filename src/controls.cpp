#include "controls.h"

#include <algorithm>

#include "game.h"
#include "settings.h"
using namespace Settings::Controls;

void Controller::scroll_callback(double y_delta) {
    game->camera->process_mouse_scroll(static_cast<float>(y_delta));
}

void Controller::cursor_position_callback(double x_delta, double y_delta) {
    game->camera->process_mouse_move(static_cast<float>(x_delta),
                                     static_cast<float>(y_delta));
}

void Controller::key_callback(int key, int action) {
    auto in_set = [&](const std::vector<int> &SET_OF_KEYS) {
        return std::find(SET_OF_KEYS.begin(), SET_OF_KEYS.end(), key) !=
               SET_OF_KEYS.end();
    };

    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (in_set(KEY_FORWARD)) game->camera->move_forward(time_delta);
        if (in_set(KEY_RIGHT)) game->camera->move_right(time_delta);
        if (in_set(KEY_BACKWARD)) game->camera->move_backward(time_delta);
        if (in_set(KEY_LEFT)) game->camera->move_left(time_delta);
    }

    // TODO: add `full_screen` key

    last_time_point = current_time;
}

void Controller::update_time() {
    last_time_point = static_cast<float>(glfwGetTime());
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

void cursor_position_callback(GLFWwindow *window, double x_delta,
                              double y_delta) {
    auto *controller =
        reinterpret_cast<Controller *>(glfwGetWindowUserPointer(window));
    if (controller) {
        controller->cursor_position_callback(x_delta, y_delta);
    } else {
        assert(false);
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
    auto *controller =
        reinterpret_cast<Controller *>(glfwGetWindowUserPointer(window));
    if (controller) {
        controller->key_callback(key, action);
    } else {
        assert(false);
    }
}
