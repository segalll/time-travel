#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Character {
private:
    glm::vec2 pos{};
    float dir;
public:
    Character();
    void move(glm::vec2 input);
    [[nodiscard]] glm::mat4 getModelMatrix() const;
};