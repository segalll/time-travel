#version 460

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D text;
uniform vec3 color;

void main() {
    fragColor = vec4(color, texture(text, texCoords).r);
}