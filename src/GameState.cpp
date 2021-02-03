#include "GameState.h"

glm::vec2 processMovementInputs(GLFWwindow* window) { // returns movement unit vector
    glm::vec2 movement{};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movement.y += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movement.y -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movement.x += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movement.x -= 1.0f;

    return movement.x == 0 && movement.y == 0 ? movement : glm::normalize(movement);
}

GameState::GameState() {
    character = Character();
}

void GameState::update(GLFWwindow* window) {
    character.move(processMovementInputs(window));
}

Character GameState::getCharacter() const {
    return character;
}