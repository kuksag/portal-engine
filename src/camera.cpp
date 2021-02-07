#include "camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/glm.hpp>

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

[[nodiscard]] glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(glm::radians(fov), ratio, display_range_near,
                            display_range_far);
}

[[nodiscard]] glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(position, position + get_forward_direction(),
                       get_up_direction());
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

void Camera::process_mouse_move(float x_delta, float y_delta, bool constraint) {
    // 'constraint = true' means that we cannot flip the screen

    horizontal_angle += mouse_sensitivity * x_delta;
    vertical_angle += mouse_sensitivity * y_delta;

    if (constraint) {
        if (vertical_angle < -M_PI_2) vertical_angle = -M_PI_2;
        if (vertical_angle > M_PI_2) vertical_angle = M_PI_2;
    }
}

void Camera::process_mouse_scroll(float y_delta) {
    // TODO: this is have to be in settings.txt
    fov += y_delta;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 120.0f) fov = 120.0f;
}
