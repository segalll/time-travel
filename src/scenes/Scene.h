#pragma once

class Scene {
public:
    virtual void onStart() = 0;
    virtual void onEnd() = 0;
    virtual void render() = 0;
};
