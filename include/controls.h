#ifndef PORTAL_ENGINE_CONTROLS_H
#define PORTAL_ENGINE_CONTROLS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

struct Game;

struct Controller {
private:
    std::unique_ptr<Game> game;
    float last_time_point;
    bool is_fullscreen;

public:
    explicit Controller(Game *game_);

    void scroll_callback(double y_delta);
    void cursor_position_callback();
    void key_callback();

    void update_time();

    void toggle_fullscreen();

    // TODO:
    //    void mouse_button_callback(int button, int action, int mods);
    //    void window_focus_callback(int focused);
};

void scroll_callback(GLFWwindow *window, double x_delta, double y_delta);

#endif    // PORTAL_ENGINE_CONTROLS_H
