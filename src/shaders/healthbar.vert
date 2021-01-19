#version 460

layout(location = 0) in vec4 vertex;

layout(std140) uniform matrices {
    mat4 projection;
};

uniform mat4 model;
uniform vec2 size;

out vec2 pos;
out vec2 fSize;

void main() {
    gl_Position = projection * model * vec4(vertex.xy * size, 0.0, 1.0);
    pos = vertex.xy * size;
    fSize = size;
}
