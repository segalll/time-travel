#pragma once

#include "Character.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum SceneEnum {
    Game,
    Pause,
    Count // will always be the number of items in enum (other than this one) as long as they are all before it
};

class GameState {
private:
    Character* character;
    bool paused;
    SceneEnum previousScene;
    SceneEnum currentScene;
public:
    explicit GameState(Character* c);
    void update(GLFWwindow* window);
    [[nodiscard]] Character getCharacter() const;
    [[nodiscard]] SceneEnum getPreviousScene() const;
    [[nodiscard]] SceneEnum getCurrentScene() const;
};