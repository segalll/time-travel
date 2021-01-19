#version 460

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D image;

void main() {
    fragColor = texture(image, texCoords);
}
