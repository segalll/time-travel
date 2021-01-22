#include "PauseMenuScene.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>

void resetFramebuffer(GLuint fbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PauseMenuScene::PauseMenuScene(Renderer* r, glm::ivec2 windowSize) : renderer(r) {
    for (int i = 0; i < 2; i++) {
        glGenFramebuffers(1, &pauseFBOs[i]);
        glGenTextures(1, &pauseTextures[i]);

        glBindFramebuffer(GL_FRAMEBUFFER, pauseFBOs[i]);
        glBindTexture(GL_TEXTURE_2D, pauseTextures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pauseTextures[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Failed to create framebuffer");
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PauseMenuScene::onStart() {
    for (int i = 0; i < 2; i++) {
        resetFramebuffer(pauseFBOs[i]);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, pauseFBOs[0]);

    renderer->getScene(SceneEnum::Game)->render();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    renderer->blur(pauseFBOs, pauseTextures, 7);
}

void PauseMenuScene::onEnd() {}

void PauseMenuScene::render() {
    GLuint fullscreenSimpleShader = ResourceManager::getShader("fullscreenSimple");
    GLuint fullscreenVignetteShader = ResourceManager::getShader("fullscreenVignette");

    glBindVertexArray(renderer->getStandardVAO());

    glUseProgram(fullscreenVignetteShader);
    glUniformMatrix4fv(glGetUniformLocation(fullscreenVignetteShader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glBindTexture(GL_TEXTURE_2D, pauseTextures[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    glUseProgram(fullscreenSimpleShader);
    glUniformMatrix4fv(glGetUniformLocation(fullscreenSimpleShader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glUniform4fv(glGetUniformLocation(fullscreenSimpleShader, "color"), 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.3f)));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    glUseProgram(fullscreenSimpleShader);
    glUniformMatrix4fv(glGetUniformLocation(fullscreenSimpleShader, "model"), 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.3333f, 1.0f))));
    glUniform4fv(glGetUniformLocation(fullscreenSimpleShader, "color"), 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 0.6f)));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    renderer->drawText("RESUME", "pauseItems", glm::vec2(-0.5f, 0.198f), glm::vec3(1.0f, 1.0f, 1.0f));
    renderer->drawText("OPTIONS", "pauseItems", glm::vec2(-0.5f, 0.066f), glm::vec3(0.5f, 0.5f, 0.5f));
    renderer->drawText("CONTROLS", "pauseItems", glm::vec2(-0.5f, -0.066f), glm::vec3(0.5f, 0.5f, 0.5f));
    renderer->drawText("EXIT GAME", "pauseItems", glm::vec2(-0.5f, -0.198f), glm::vec3(0.5f, 0.5f, 0.5f));
}
