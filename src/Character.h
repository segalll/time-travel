#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Character {
private:
    glm::vec2 pos{};
    float dir;
    float health;
    float maxHealth;
    std::vector<unsigned int> inventory;
public:
    Character();
    void move(glm::vec2 input);
    [[nodiscard]] glm::mat4 getModelMatrix() const;
    [[nodiscard]] float getHealthPercentage() const;
};