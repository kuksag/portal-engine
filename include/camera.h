#ifndef PORTAL_ENGINE_H
#define PORTAL_ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "settings.h"
using namespace Settings::Camera;

struct Camera {
private:
    glm::vec3 position;
    float horizontal_angle;
    float vertical_angle;

    float move_speed;
    float mouse_sensitivity;
    float fov;
    float ratio;
    float display_range_near;
    float display_range_far;

public:
    explicit Camera(glm::vec3 position_ = glm::vec3(X_POS, Y_POS, Z_POS),
                    float horizontal_angle_ = HORIZONTAL_ANGLE,
                    float vertical_angle_ = VERTICAL_ANGLE)
        : position(position_),
          horizontal_angle(horizontal_angle_),
          vertical_angle(vertical_angle_),
          move_speed(MOVE_SPEED),
          mouse_sensitivity(MOUSE_SENSITIVITY),
          fov(FOV),
          ratio(RATIO),
          display_range_near(DISPLAY_RANGE_NEAR),
          display_range_far(DISPLAY_RANGE_FAR) {}

    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_forward_direction() const;
    [[nodiscard]] glm::vec3 get_right_direction() const;
    [[nodiscard]] glm::vec3 get_up_direction() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;
    [[nodiscard]] glm::mat4 get_view_matrix() const;

    void move_forward(float time_delta);
    void move_backward(float time_delta);
    void move_right(float time_delta);
    void move_left(float time_delta);

    void process_mouse_move(float x_delta, float y_delta,
                            bool constraint = true);
    void process_mouse_scroll(float y_delta);
};

#endif    // PORTAL_ENGINE_CAMERA_H
