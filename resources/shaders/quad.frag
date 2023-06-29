#version 150

in vec2 pass_Coordinates;

uniform sampler2D ColorTexture;
uniform sampler2D DepthTexture;

out vec4 out_Color;

float near = 0.1;
float far = 100.0;

float linearizeDepth(float depth) {
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

void main() {
    out_Color = vec4(1.0) - texture(ColorTexture, pass_Coordinates);
}