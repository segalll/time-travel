#pragma once

#include "Scene.h"

#include <GL/glew.h>

#include "../Character.h"

class GameScene : public virtual Scene {
private:
    GLuint standardVao;
    GLuint floorVao;
    Character* character;
public:
    GameScene(GLuint standardVAO, Character* c);
    void onStart() override;
    void onEnd() override;
    void render() override;
};