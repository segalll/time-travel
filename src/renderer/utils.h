#pragma once

#include <GL/glew.h>

#include <array>

GLuint createRenderData(std::array<float, 24> vertices); // static draw
GLuint createRenderData(); // dynamic draw