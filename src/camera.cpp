#include "camera.h"

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

#include "settings.h"
using namespace Settings::Camera;

void Camera::update(float dt) {
     speed += acceleration * dt;
     position += speed * dt;
     if (position.y < 1) {
         position.y = 1;
         speed.y = 0;
         is_on_floor = true;
     }
}

void Camera::jump() {
    if (is_on_floor) {
        speed += jump_speed;
        is_on_floor = false;
    }
}

[[nodiscard]] glm::vec3 Camera::get_position() const { return position; }

// conversion from spherical coords to Cartesian
[[nodiscard]] glm::vec3 Camera::get_forward_direction() const {
    return glm::normalize(
        glm::vec3(std::cos(vertical_angle) * std::sin(horizontal_angle),
                  std::sin(vertical_angle),
                  std::cos(vertical_angle) * std::cos(horizontal_angle)));
}

[[nodiscard]] glm::vec3 Camera::get_right_direction() const {
    // where M_PI_2 = Pi/2
    return glm::normalize(glm::vec3(std::sin(horizontal_angle - M_PI_2), 0,
                                    std::cos(horizontal_angle - M_PI_2)));
}

[[nodiscard]] glm::vec3 Camera::get_up_direction() const {
    return glm::normalize(
        glm::cross(get_right_direction(), get_forward_direction()));
}

[[nodiscard]] glm::mat4 Camera::get_view_matrix() const {
    if (custom_state_view_matrix) {
        return custom_view_matrix;
    } else {
        return glm::lookAt(position, position + get_forward_direction(),
                           get_up_direction());
    }
}

[[nodiscard]] glm::mat4 Camera::get_projection_matrix() const {
    if (custom_state_projection_matrix) {
        return custom_projection_matrix;
    } else {
        return glm::perspective(glm::radians(fov), ratio, display_range_near,
                                display_range_far);
    }
}

void Camera::move_forward(float time_delta) {
    position += move_speed * time_delta * get_forward_direction();
}

void Camera::move_backward(float time_delta) {
    position -= move_speed * time_delta * get_forward_direction();
}

void Camera::move_right(float time_delta) {
    position += move_speed * time_delta * get_right_direction();
}

void Camera::move_left(float time_delta) {
    position -= move_speed * time_delta * get_right_direction();
}

void Camera::move_up(float time_delta) {
    position += move_speed * time_delta * get_up_direction();
}

void Camera::move_down(float time_delta) {
    position -= move_speed * time_delta * get_up_direction();
}

void Camera::process_mouse_move(float x_delta, float y_delta, float time_delta,
                                bool constraint) {
    // 'constraint = true' means that we cannot flip the screen

    horizontal_angle += mouse_sensitivity * time_delta * x_delta;
    vertical_angle += mouse_sensitivity * time_delta * y_delta;

    if (constraint) {
        if (vertical_angle < VERTICAL_ANGLE_MIN)
            vertical_angle = VERTICAL_ANGLE_MIN;
        if (vertical_angle > VERTICAL_ANGLE_MAX)
            vertical_angle = VERTICAL_ANGLE_MAX;
    }
}

void Camera::process_mouse_scroll(float y_delta) {
    fov += scroll_sensitivity * y_delta;
    if (fov < FOV_MIN) fov = FOV_MIN;
    if (fov > FOV_MAX) fov = FOV_MAX;
}

void Camera::set_view_matrix(glm::mat4 data, bool recalculate) {
    if (recalculate) {
        // https://stackoverflow.com/questions/349050/calculating-a-lookat-matrix
        glm::mat4 axis =
            glm::mat4(data[0], data[1], data[2], glm::vec4(glm::vec3(0), 1));
        glm::mat4 negative_eye = glm::inverse(axis) * data;
        position = -negative_eye[3];
        vertical_angle = std::asin(-data[1][2]);
        horizontal_angle =
            std::atan2(data[0][0], data[2][0]) + static_cast<float>(M_PI_2);
    } else {
        custom_state_view_matrix = true;
        custom_view_matrix = data;
    }
}
void Camera::set_projection_matrix([[maybe_unused]] glm::mat4 data) {}
