#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameState.h"
#include "ResourceManager.h"

class Renderer {
private:
    ResourceManager resourceManager;
    GLuint ubo;
    GLuint standardVao;
public:
    explicit Renderer(GLFWwindow* window);
    [[nodiscard]] unsigned int getUBO() const;
    void renderState(GLFWwindow* window, const GameState& state);
};
