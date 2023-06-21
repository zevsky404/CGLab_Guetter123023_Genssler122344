#version 150
#extension GL_NV_shadow_samplers_cube : enable
in vec3 pass_Coordinates;

uniform samplerCube TextureSampler;

out vec4 out_Color;

void main() {
    out_Color = textureCube(TextureSampler, pass_Coordinates);
}