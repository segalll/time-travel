#pragma once

#include "Scene.h"

#include "../Character.h"
#include "../renderer/Renderer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>

class PauseMenuScene : public virtual Scene {
private:
    Renderer* renderer;
    std::array<GLuint, 2> pauseFBOs{};
    std::array<GLuint, 2> pauseTextures{};
public:
    PauseMenuScene(Renderer* r, glm::ivec2 windowSize);
    void onStart() override;
    void onEnd() override;
    void render() override;
};