#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../scenes/PauseMenuScene.h"
#include "../scenes/GameScene.h"
#include "utils.h"

#include <array>
#include <stdexcept>
#include <algorithm>

namespace {
    void setProjectionMatrix(GLuint ubo, int width, int height) {
        glm::mat4 projection = glm::ortho(-(float)width / (float)height, (float)width / (float)height, -1.0f, 1.0f);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    GLuint createUniformBuffer(GLFWwindow* window) {
        GLuint ubo;
        glGenBuffers(1, &ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(glm::mat4));

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        setProjectionMatrix(ubo, width, height);

        return ubo;
    }

    void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        if (width == 0 || height == 0) return;

        auto* renderer = (Renderer*)glfwGetWindowUserPointer(window);
        setProjectionMatrix(renderer->getUBO(), width, height);
        glViewport(0, 0, width, height);
        renderer->setScene(SceneEnum::Pause, std::make_shared<PauseMenuScene>(renderer, glm::ivec2(width, height)));
    }
}

Renderer::Renderer(GLFWwindow* window, Character* character) {
    glfwSetWindowUserPointer(window, this);

    ubo = createUniformBuffer(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::array<float, 24> standardVertices = {
        // pos      // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f
    };
    standardVao = createRenderData(standardVertices);

    textVbo = 0;
    glBindVertexArray(0);
    glGenBuffers(1, &textVbo);
    glBindBuffer(GL_ARRAY_BUFFER, textVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    ResourceManager::initialize();
    ResourceManager::loadShader("sprite");
    ResourceManager::loadShader("healthbar");
    ResourceManager::loadShader("fullscreen", "blur", "blur");
    ResourceManager::loadShader("simple");
    ResourceManager::loadShader("fullscreen", "sprite", "fullscreenSprite");
    ResourceManager::loadShader("fullscreen", "simple", "fullscreenSimple");
    ResourceManager::loadShader("sprite", "text", "text");
    ResourceManager::loadTexture("floor.png", false, "floor");
    ResourceManager::loadTexture("hoodedpurpchar.png", true, "character");
    ResourceManager::loadFont("OpenSans-Regular.ttf", 48, "pauseItems");

    initializeScenes(window, character);
}

Renderer::~Renderer() {
    ResourceManager::clear();
}

void Renderer::initializeScenes(GLFWwindow* window, Character* character) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    scenes[SceneEnum::Pause] = std::make_shared<PauseMenuScene>(this, glm::ivec2(width, height));
    scenes[SceneEnum::Game] = std::make_shared<GameScene>(standardVao, character);
}

GLuint Renderer::getUBO() const {
    return ubo;
}

GLuint Renderer::getStandardVAO() const {
    return standardVao;
}

std::shared_ptr<Scene> Renderer::getScene(SceneEnum sceneEnum) {
    return scenes[sceneEnum];
}

void Renderer::setScene(SceneEnum sceneEnum, const std::shared_ptr<Scene>& newScene) {
    scenes[sceneEnum] = newScene;
}

void Renderer::drawText(const std::string& text, const std::string& font, glm::vec2 pos) const {
    std::vector<float> vertices;

    const std::array<FontChar, 128> fontChars = ResourceManager::getFont(font);

    const float sx = 2.0f / 1440.0f; // not separate scale factors because of my orthographic matrix extending the x

    float x = 0.0f;
    float y = 0.0f;
    for (char c : text) {
        const float vx = x + fontChars[c].bearing.x * sx;
        const float vy = y + fontChars[c].bearing.y * sx;
        const float w = fontChars[c].size.x * sx;
        const float h = fontChars[c].size.y * sx;
        const float tx = fontChars[c].tx;
        const glm::vec2 td = fontChars[c].texSize;
        std::vector<float> quad = {
            vx, vy, tx, 0.0f,
            vx + w, vy, tx + td.x, 0.0f,
            vx + w, vy - h, tx + td.x, td.y,
            vx + w, vy - h, tx + td.x, td.y,
            vx, vy - h, tx, td.y,
            vx, vy, tx, 0.0f
        };
        vertices.insert(vertices.begin(), quad.begin(), quad.end());
        x += float(fontChars[c].advance.x >> 6) * sx;
        y += float(fontChars[c].advance.y >> 6) * sx;
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, textVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture(font));
    GLuint textShader = ResourceManager::getShader("text");
    glUseProgram(textShader);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y - (0.02222f), 0.0f)); // pos.x - x / 2 for center x
    glUniformMatrix4fv(glGetUniformLocation(textShader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::blur(std::array<GLuint, 2> fbos, std::array<GLuint, 2> textures, int iterations) const {
    bool horizontal = true;
    glBindVertexArray(standardVao);
    GLuint blurShader = ResourceManager::getShader("blur");
    glUseProgram(blurShader);
    for (int i = 0; i < iterations * 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbos[horizontal]);
        glBindTexture(GL_TEXTURE_2D, textures[!horizontal]);
        glUniformMatrix4fv(glGetUniformLocation(blurShader, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        glUniform1i(glGetUniformLocation(blurShader, "horizontal"), horizontal);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        horizontal = !horizontal;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderState(GLFWwindow* window, const GameState& state) {
    glClear(GL_COLOR_BUFFER_BIT);

    if (state.getCurrentScene() != state.getPreviousScene()) {
        scenes[state.getCurrentScene()]->onStart();
        scenes[state.getPreviousScene()]->onEnd();
    }

    scenes[state.getCurrentScene()]->render();

    glfwSwapBuffers(window);
    glfwPollEvents();
}
