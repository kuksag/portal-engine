#ifndef PORTAL_ENGINE_CAMERA_H
#define PORTAL_ENGINE_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace {

// TODO: make setting.txt
// TODO: clang-tidy glm3::vec3 "init of static storage duration may throw ..."

glm::vec3 initial_position = glm::vec3(0.0f, 0.0f, 2.0f);

// initial horizontal angle : toward -Z, radians
float initial_horizontal_angle = 3.14f;

// initial vertical angle : none
float initial_vertical_angle = 0.0f;

// initial Field Of View, degree
float initial_fov = 45.0f;

float initial_movement_speed = 3.0f;    // 3 units / second
float initial_mouse_sensitivity = 0.005f;
float initial_ratio = 4.0f / 3.0f;
float initial_display_range_near = 0.1f;
float initial_display_range_far = 100.0f;

}    // namespace

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
    // looks terrible, there has to be a better way
    explicit Camera(glm::vec3 position_ = initial_position,
                    float horizontal_angle_ = initial_horizontal_angle,
                    float vertical_angle_ = initial_vertical_angle)
        : position(position_),
          horizontal_angle(horizontal_angle_),
          vertical_angle(vertical_angle_),
          move_speed(initial_movement_speed),
          mouse_sensitivity(initial_mouse_sensitivity),
          fov(initial_fov),
          ratio(initial_ratio),
          display_range_near(initial_display_range_near),
          display_range_far(initial_display_range_far) {}

    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_forward_direction() const;
    [[nodiscard]] glm::vec3 get_right_direction() const;
    [[nodiscard]] glm::vec3 get_up_direction() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;
    [[nodiscard]] glm::mat4 get_view_matrix() const;

    // TODO: I dont know where to store time_delta; need to discuss
    void move_forward(float time_delta);
    void move_backward(float time_delta);
    void move_right(float time_delta);
    void move_left(float time_delta);

    void process_mouse_move(float x_delta, float y_delta,
                            bool constraint = true);
    void process_mouse_scroll(float y_delta);
};

#endif    // PORTAL_ENGINE_CAMERA_H
