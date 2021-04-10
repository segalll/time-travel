#pragma once

#include <GL/gl.h>

#include <array>

class Chunk {
private:
    std::array<unsigned short, 16 * 16> tiles;
    GLuint vbo;
};