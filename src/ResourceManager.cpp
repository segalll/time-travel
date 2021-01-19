#include "ResourceManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, GLuint> ResourceManager::shaders;
std::map<std::string, GLuint> ResourceManager::textures;
std::map<std::string, std::array<FontChar, 128>> ResourceManager::fonts;
FT_Library ResourceManager::ft;

void ResourceManager::initialize() {
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("Failed to initialize freetype");
    }
}

GLuint ResourceManager::loadShader(const std::string& name) {
    std::string vFile = "shaders/" + name + ".vert";
    std::string fFile = "shaders/" + name + ".frag";
    shaders[name] = loadShaderFromFile(vFile.c_str(), fFile.c_str());
    return shaders[name];
}

GLuint ResourceManager::loadShader(const std::string& vShaderName, const std::string& fShaderName, const std::string& name) {
    std::string vFile = "shaders/" + vShaderName + ".vert";
    std::string fFile = "shaders/" + fShaderName + ".frag";
    shaders[name] = loadShaderFromFile(vFile.c_str(), fFile.c_str());
    return shaders[name];
}

GLuint ResourceManager::getShader(const std::string& name) {
    return shaders[name];
}

GLuint ResourceManager::loadTexture(const char* file, bool alpha, const std::string& name) {
    std::string actualFile = file;
    actualFile = "textures/" + actualFile;
    textures[name] = loadTextureFromFile(actualFile.c_str(), alpha);
    return textures[name];
}

GLuint ResourceManager::loadTexture(GLuint texture, const std::string &name) {
    textures[name] = texture;
    return texture;
}

GLuint ResourceManager::getTexture(const std::string& name) {
    return textures[name];
}

std::array<FontChar, 128> ResourceManager::loadFont(const char *file, int size, const std::string &name) {
    std::string actualFile = file;
    actualFile = "fonts/" + actualFile;
    fonts[name] = loadFontFromFile(actualFile.c_str(), size, name);
    return fonts[name];
}

std::array<FontChar, 128> ResourceManager::getFont(const std::string &name) {
    return fonts[name];
}

void ResourceManager::clear() {
    for (const auto& i : shaders) {
        glDeleteProgram(i.second);
    }
    for (const auto& i : textures) {
        glDeleteTextures(1, &i.second);
    }
}

GLuint ResourceManager::compileShader(const char* shaderSource, GLenum shaderType) {
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

GLuint ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile) {
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
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to read shader files");
    }

    GLuint shaderProgram = glCreateProgram();

    GLuint vs = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fs = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);

    glLinkProgram(shaderProgram);

    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&isLinked);
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

GLuint ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
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

std::array<FontChar, 128> ResourceManager::loadFontFromFile(const char* filename, int size, const std::string& name) {
    FT_Face face;
    if (FT_New_Face(ft, filename, 0, &face)) {
        throw std::runtime_error("Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::array<FontChar, 128> characters{};

    unsigned int w = 0;
    unsigned int h = 0;
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_BITMAP_METRICS_ONLY)) {
            throw std::runtime_error("Failed to load glyph");
        }
        w += face->glyph->bitmap.width + 5;
        h = std::max(face->glyph->bitmap.rows, h);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int x = 0;
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            throw std::runtime_error("Failed to load glyph");
        }
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        FontChar character = {
            (float)x / (float)w,
            glm::vec2((float)face->glyph->bitmap.width / (float)w, (float)face->glyph->bitmap.rows / (float)h),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            glm::ivec2(face->glyph->advance.x, face->glyph->advance.y)
        };
        characters[c] = character;

        x += face->glyph->bitmap.width + 5;
    }
    ResourceManager::loadTexture(texture, name);
    return characters;
}