#ifndef PORTAL_ENGINE_GAME_H
#define PORTAL_ENGINE_GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "camera.h"
#include "controls.h"

struct Controller;

struct Game {
private:
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Controller> controller;
    GLFWwindow *window;

    // TODO: add `RenderEngine` object, that actually draws graphics

    void window_initialise();

public:
    Game();

    void run();

    friend struct Controller;
};

#endif    // PORTAL_ENGINE_GAME_H
