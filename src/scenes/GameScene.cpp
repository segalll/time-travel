#include "GameScene.h"

#include <glm/gtc/type_ptr.hpp>

#include <array>

#include "../renderer/utils.h"
#include "../ResourceManager.h"

GameScene::GameScene(GLuint standardVAO, Character* c) : standardVao(standardVAO), character(c) {
    std::array<float, 24> floorVertices = {
        // pos      // tex
        -2.0f, -2.0f, 0.0f, 0.0f,
        2.0f, -2.0f, 64.0f, 0.0f,
        2.0f, 2.0f, 64.0f, 64.0f,
        2.0f, 2.0f, 64.0f, 64.0f,
        -2.0f, 2.0f, 0.0f, 64.0f,
        -2.0f, -2.0f, 0.0f, 0.0f
    };
    floorVao = createRenderData(floorVertices);
}

void GameScene::onStart() {}

void GameScene::onEnd() {}

void GameScene::render() {
    glBindVertexArray(floorVao);

    GLuint spriteShader = ResourceManager::getShader("sprite");

    glUseProgram(spriteShader);
    glUniformMatrix4fv(glGetUniformLocation(spriteShader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("floor"));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    glBindVertexArray(standardVao);

    glUseProgram(spriteShader);
    glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("character"));
    glUniformMatrix4fv(glGetUniformLocation(spriteShader, "model"), 1, GL_FALSE, glm::value_ptr(character->getModelMatrix()));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    GLuint healthbarShader = ResourceManager::getShader("healthbar");
    glUseProgram(healthbarShader);
    glUniformMatrix4fv(glGetUniformLocation(healthbarShader, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.85f, 0.0f))));
    glUniform2fv(glGetUniformLocation(healthbarShader, "size"), 1, glm::value_ptr(glm::vec2(0.5f, 0.05f)));
    glUniform1f(glGetUniformLocation(healthbarShader, "progress"), character->getHealthPercentage());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
}
