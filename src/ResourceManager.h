#pragma once

#include <glm/glm.hpp>

#include <map>
#include <string>
#include <array>

class ResourceManager {
private:
    std::map<std::string, unsigned int> shaders;
    std::map<std::string, unsigned int> textures;
public:
    ResourceManager() = default;
    ~ResourceManager();
    unsigned int loadShader(const std::string& vShaderName, const std::string& fShaderName, const std::string& name);
    unsigned int getShader(const std::string& name);
    unsigned int loadTexture(const std::string& file, bool alpha, const std::string& name);
    unsigned int getTexture(const std::string& name);
};