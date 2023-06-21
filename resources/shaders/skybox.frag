#version 150
in vec3 pass_Coordinates;

uniform samplerCube TextureSampler;

out vec4 out_Color;

void main() {
    out_Color = texture(TextureSampler, pass_Coordinates);
}