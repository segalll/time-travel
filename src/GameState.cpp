#include "GameState.h"

#include <set>

glm::vec2 processMovementInputs(GLFWwindow* window) { // returns movement unit vector
    glm::vec2 movement{};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movement.y += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movement.y -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movement.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movement.x -= 1.0f;

    return movement.x == 0 && movement.y == 0 ? movement : glm::normalize(movement);
}

bool keyToggled(GLFWwindow* window, int key) {
    static std::set<int> previousKeys;
    bool keyPressed = glfwGetKey(window, key) == GLFW_PRESS;
    bool toggled = keyPressed && previousKeys.find(key) == previousKeys.end(); // key pressed and key not previously pressed
    previousKeys.clear();
    if (keyPressed) previousKeys.insert(key);
    return toggled;
}

GameState::GameState(Character* c) {
    character = c;
    paused = false;
    previousScene = SceneEnum::Game;
    currentScene = SceneEnum::Game;
}

void GameState::update(GLFWwindow* window) {
    paused ^= keyToggled(window, GLFW_KEY_ESCAPE);
    previousScene = currentScene;
    if (paused) {
        currentScene = SceneEnum::Pause;
    } else {
        currentScene = SceneEnum::Game;
    }
    if (!paused) character->move(processMovementInputs(window));
}

Character GameState::getCharacter() const {
    return *character;
}

SceneEnum GameState::getPreviousScene() const {
    return previousScene;
}

SceneEnum GameState::getCurrentScene() const {
    return currentScene;
}