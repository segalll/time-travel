#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "GameState.h"
#include "renderer/Renderer.h"

void mainLoop(GLFWwindow* window) {
    GameState gameState;
    Renderer renderer(window);
    while (!glfwWindowShouldClose(window)) {
        gameState.update(window);
        renderer.renderState(window, gameState);
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW failed to initialize" << std::endl;
        return -1;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* v = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(v->width, v->height, "time travel", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW failed to initialize" << std::endl;
        glfwTerminate();
        return -1;
    }

    mainLoop(window);

    return 0;
}
