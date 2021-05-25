#ifndef PORTAL_ENGINE_SETTINGS_H
#define PORTAL_ENGINE_SETTINGS_H

#include <cmath>
#include <vector>
namespace Settings {

namespace Controls {
// TODO:
//  Clang-Tidy: Initialization of '***' with static storage duration may
//  throw an exception that cannot be caught

const std::vector<int> KEY_FORWARD = {GLFW_KEY_W, GLFW_KEY_UP};
const std::vector<int> KEY_RIGHT = {GLFW_KEY_D, GLFW_KEY_RIGHT};
const std::vector<int> KEY_BACKWARD = {GLFW_KEY_S, GLFW_KEY_DOWN};
const std::vector<int> KEY_LEFT = {GLFW_KEY_A, GLFW_KEY_LEFT};

const std::vector<int> KEY_FULLSCREEN = {GLFW_KEY_F11};
const std::vector<int> KEY_WINDOWED = {GLFW_KEY_F12};

const int REFRESH_RATE = 60;
}    // namespace Controls

namespace Window {
const int WIDTH = 1024, HEIGHT = 768;
const int FULL_WIDTH = 1920, FULL_HEIGHT = 1200;
const char TITLE[] = "Portal Engine";
}    // namespace Window

namespace Camera {
const float POS_X = 0.0f;
const float POS_Y = 0.0f;
const float POS_Z = 2.0f;

// radians
const float HORIZONTAL_ANGLE = M_PI;
const float VERTICAL_ANGLE = 0;
const float VERTICAL_ANGLE_MIN = -M_PI_2;
const float VERTICAL_ANGLE_MAX = M_PI_2;

// degrees
const float FOV = 45.0f;
const float FOV_MIN = 1.0f;
const float FOV_MAX = 120.0f;

const float RATIO = 4.0f / 3.0f;

const float MOVE_SPEED = 3.0f;
const float MOUSE_SENSITIVITY = 0.25f;
const float SCROLL_SENSITIVITY = 1.0f;

const float DISPLAY_RANGE_NEAR = 0.1f;
const float DISPLAY_RANGE_FAR = 100.0f;
}    // namespace Camera

namespace Portal {
const int MAX_RECURSION_LEVEL = 4;
}

}    // namespace Settings

#endif    // PORTAL_ENGINE_SETTINGS_H
