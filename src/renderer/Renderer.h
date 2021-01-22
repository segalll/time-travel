#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../GameState.h"
#include "../scenes/Scene.h"
#include "../ResourceManager.h"

#include <array>
#include <string>
#include <map>
#include <memory>

class Renderer {
private:
    GLuint ubo;
    GLuint standardVao;
    GLuint textVbo;
    std::array<std::shared_ptr<Scene>, (int)SceneEnum::Count> scenes;
    void initializeScenes(GLFWwindow* window, Character* character);
public:
    explicit Renderer(GLFWwindow* window, Character* character);
    ~Renderer();
    [[nodiscard]] GLuint getUBO() const;
    [[nodiscard]] GLuint getStandardVAO() const;
    std::shared_ptr<Scene> getScene(SceneEnum sceneEnum);
    void setScene(SceneEnum sceneEnum, const std::shared_ptr<Scene>& newScene);
    void drawText(const std::string& text, const std::string& font, glm::vec2 pos, glm::vec3 color) const;
    void blur(std::array<GLuint, 2> fbos, std::array<GLuint, 2> textures, int iterations) const;
    void renderState(GLFWwindow* window, const GameState& state);
};
