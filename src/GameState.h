#pragma once

#include "Character.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GameState {
private:
    Character character;
public:
    GameState();
    void update(GLFWwindow* window);
    [[nodiscard]] Character getCharacter() const;
};