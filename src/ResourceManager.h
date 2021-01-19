#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>
#include <array>

struct FontChar {
    float tx;
    glm::vec2 texSize;
    glm::ivec2 size;
    glm::ivec2 bearing;
    glm::ivec2 advance;
};

class ResourceManager {
private:
    static FT_Library ft;
    static GLuint compileShader(const char* shaderSource, GLenum shaderType);
    static GLuint loadShaderFromFile(const char* vShaderFile, const char* fShaderFile);
    static GLuint loadTextureFromFile(const char* file, bool alpha);
    static std::array<FontChar, 128> loadFontFromFile(const char* file, int size, const std::string& name);
public:
    static std::map<std::string, GLuint> shaders;
    static std::map<std::string, GLuint> textures;
    static std::map<std::string, std::array<FontChar, 128>> fonts;

    static void initialize();
    static GLuint loadShader(const std::string& name);
    static GLuint loadShader(const std::string& vShaderName, const std::string& fShaderName, const std::string& name);
    static GLuint getShader(const std::string& name);
    static GLuint loadTexture(const char* file, bool alpha, const std::string& name);
    static GLuint loadTexture(GLuint texture, const std::string& name);
    static GLuint getTexture(const std::string& name);
    static std::array<FontChar, 128> loadFont(const char* file, int size, const std::string& name);
    static std::array<FontChar, 128> getFont(const std::string& name);

    static void clear();
};