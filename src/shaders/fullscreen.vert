#version 460

layout(location = 0) in vec4 vertex;

uniform mat4 model = mat4(1.0);

out vec2 texCoords;

void main() {
    texCoords = vertex.zw;
    gl_Position = model * vec4(vertex.xy, 0.0, 1.0);
}
