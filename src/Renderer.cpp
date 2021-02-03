#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    }

    GLuint createRenderData(std::vector<float> vertices) {
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr); // this assumes that we have texcoords
        glBindVertexArray(0);
        return vao;
    }
}

Renderer::Renderer(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);

    ubo = createUniformBuffer(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<float> standardVertices = {
        // pos      // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f
    };
    standardVao = createRenderData(standardVertices);

    resourceManager.loadShader("sprite.vert", "sprite.frag", "sprite");
    resourceManager.loadTexture("hoodedpurpchar.png", true, "character");
}

Renderer::~Renderer() {
    resourceManager.clear();
}

GLuint Renderer::getUBO() const {
    return ubo;
}

void Renderer::renderState(GLFWwindow* window, const GameState& state) {
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(standardVao);

    GLuint spriteShader = resourceManager.getShader("sprite");
    glUseProgram(spriteShader);
    glBindTexture(GL_TEXTURE_2D, resourceManager.getTexture("character"));
    glUniformMatrix4fv(glGetUniformLocation(spriteShader, "model"), 1, GL_FALSE, glm::value_ptr(state.getCharacter().getModelMatrix()));
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
