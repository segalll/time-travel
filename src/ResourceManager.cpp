#include "ResourceManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace {
    GLuint compileShader(const char* shaderSource, GLenum shaderType) {
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);

            std::cout << "Failed to compile shader" << std::endl;

            throw std::runtime_error(infoLog);
        }
        return shader;
    }

    GLuint loadShaderFromFile(const char* vShaderFile, const char* fShaderFile) {
        std::string vertexCode;
        std::string fragmentCode;
        try {
            std::ifstream vertexFile(vShaderFile);
            std::ifstream fragmentFile(fShaderFile);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vertexFile.rdbuf();
            fShaderStream << fragmentFile.rdbuf();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (const std::exception &e) {
            throw std::runtime_error("Failed to read shader files");
        }

        GLuint shaderProgram = glCreateProgram();

        GLuint vs = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        GLuint fs = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);

        glLinkProgram(shaderProgram);

        GLint isLinked = 0;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *) &isLinked);
        if (isLinked == GL_FALSE) {
            std::cout << "Shader program failed to link" << std::endl;

            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);

            glDeleteProgram(shaderProgram);
            glDeleteShader(vs);
            glDeleteShader(fs);

            std::cout << "Shader program failed to link" << std::endl;
            throw std::runtime_error(infoLog);
        }
        glDeleteShader(vs);
        glDeleteShader(fs);

        return shaderProgram;
    }

    GLuint loadTextureFromFile(const char* file, bool alpha) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
        GLint format = alpha ? GL_RGBA : GL_RGB;
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            throw std::runtime_error("Failed to load texture");
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }
}

GLuint ResourceManager::loadShader(const std::string& vShaderName, const std::string& fShaderName, const std::string& name) {
    std::string vFile = "shaders/" + vShaderName;
    std::string fFile = "shaders/" + fShaderName;
    shaders[name] = loadShaderFromFile(vFile.c_str(), fFile.c_str());
    return shaders[name];
}

GLuint ResourceManager::getShader(const std::string& name) {
    return shaders[name];
}

GLuint ResourceManager::loadTexture(const std::string& file, bool alpha, const std::string& name) {
    std::string actualFile = "textures/" + file;
    textures[name] = loadTextureFromFile(actualFile.c_str(), alpha);
    return textures[name];
}

GLuint ResourceManager::getTexture(const std::string& name) {
    return textures[name];
}

void ResourceManager::clear() {
    for (const auto& i : shaders) {
        glDeleteProgram(i.second);
    }
    for (const auto& i : textures) {
        glDeleteTextures(1, &i.second);
    }
}