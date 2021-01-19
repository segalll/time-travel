#version 460

uniform float progress;
uniform float borderSize = 0.02;

in vec2 pos;
in vec2 fSize;

out vec4 fragColor;

void main() {
    if (pos.y > fSize.y - borderSize || pos.y < borderSize - fSize.y || pos.x > fSize.x - borderSize|| pos.x < borderSize - fSize.x) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        if (pos.x > (progress * 2.0 * (fSize.x - borderSize)) - (fSize.x - borderSize)) {
            fragColor = vec4(1.0, 1.0, 1.0, 0.0);
        } else {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    }
}