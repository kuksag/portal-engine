#ifndef PORTAL_ENGINE_SETTINGS_H
#define PORTAL_ENGINE_SETTINGS_H

namespace Settings {

namespace Window {
const int WIDTH = 1024, HEIGHT = 768;
}

namespace Camera {
const float X_POS = 0.0f;
const float Y_POS = 0.0f;
const float Z_POS = 2.0f;

const float HORIZONTAL_ANGLE = M_PI;
const float VERTICAL_ANGLE = 0;

const float FOV = 45.0f;
const float RATIO = 4.0f / 3.0f;

const float MOVE_SPEED = 3.0f;
const float MOUSE_SENSITIVITY = 0.005f;

const float DISPLAY_RANGE_NEAR = 0.1f;
const float DISPLAY_RANGE_FAR = 100.0f;
}
}    // namespace Settings

#endif    // PORTAL_ENGINE_SETTINGS_H
