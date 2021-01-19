#version 460

layout(location = 0) in vec4 vertex;

layout(std140) uniform matrices {
    mat4 projection;
};

out vec2 texCoords;

uniform mat4 model;

void main() {
    texCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
