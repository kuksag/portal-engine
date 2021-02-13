#include "controls.h"

#include "game.h"
#include "settings.h"
using namespace Settings::Controls;

void Controller::scroll_callback(double y_delta) {
    game->camera->process_mouse_scroll(static_cast<float>(y_delta));
}

void Controller::cursor_position_callback() {
    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;
    int width, height;
    glfwGetWindowSize(game->window, &width, &height);

    double x_mouse = 0;
    double y_mouse = 0;
    glfwGetCursorPos(game->window, &x_mouse, &y_mouse);

    auto x_delta = static_cast<float>(width / 2.0 - x_mouse);
    auto y_delta = static_cast<float>(height / 2.0 - y_mouse);

    game->camera->process_mouse_move(static_cast<float>(x_delta),
                                     static_cast<float>(y_delta), time_delta);

    glfwSetCursorPos(game->window, width / 2.0, height / 2.0);
}

void Controller::key_callback() {
    auto current_time = static_cast<float>(glfwGetTime());
    auto time_delta = current_time - last_time_point;

    // move camera forward
    for (auto &key : KEY_FORWARD) {
        if (glfwGetKey(game->window, key) == GLFW_PRESS) {
            game->camera->move_forward(time_delta);
        }
    }

    // move camera rightward
    for (auto &key : KEY_RIGHT) {
        if (glfwGetKey(game->window, key) == GLFW_PRESS) {
            game->camera->move_right(time_delta);
        }
    }

    // move camera backward
    for (auto &key : KEY_BACKWARD) {
        if (glfwGetKey(game->window, key) == GLFW_PRESS) {
            game->camera->move_backward(time_delta);
        }
    }

    // move camera leftward
    for (auto &key : KEY_LEFT) {
        if (glfwGetKey(game->window, key) == GLFW_PRESS) {
            game->camera->move_left(time_delta);
        }
    }

    // TODO: add `full_screen` key
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
