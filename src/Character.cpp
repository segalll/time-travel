#include "Character.h"

Character::Character() {
    pos = glm::vec2(0.0f, 0.0f);
    dir = 1.0f;
    health = 1.0f;
    maxHealth = 1.0f;
}

void Character::move(glm::vec2 input) {
    const float speed = 0.05f;

    pos += input * speed;
    dir = input.x == 0.0f ? dir : input.x > 0.0f ? 1.0f : -1.0f; // lol
}

glm::mat4 Character::getModelMatrix() const {
    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(dir * 0.1f, 0.1f, 0.05f));
    model = glm::translate(model, glm::vec3(dir * pos.x, pos.y, 0.0f));
    return model;
}

float Character::getHealthPercentage() const {
    return health / maxHealth;
}