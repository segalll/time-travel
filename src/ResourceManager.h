#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <map>
#include <string>
#include <array>

class ResourceManager {
private:
    std::map<std::string, GLuint> shaders;
    std::map<std::string, GLuint> textures;
public:
    ResourceManager() = default;
    GLuint loadShader(const std::string& vShaderName, const std::string& fShaderName, const std::string& name);
    GLuint getShader(const std::string& name);
    GLuint loadTexture(const std::string& file, bool alpha, const std::string& name);
    GLuint getTexture(const std::string& name);

    void clear();
};