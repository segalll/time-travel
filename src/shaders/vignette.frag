#version 460

in vec2 texCoords;
in vec2 vPos;
out vec4 fragColor;

uniform sampler2D image;

const float radius = 1.25;
const float softness = 0.75;

void main() {
    vec4 texColor = texture(image, texCoords);
    float vignette = smoothstep(radius, radius - softness, length(vPos));
    texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.5);
    fragColor = texColor;
}
