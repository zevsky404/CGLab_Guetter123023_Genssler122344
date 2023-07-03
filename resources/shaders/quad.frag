#version 460

#define gaussian_blur mat3(1, 2, 1, 2, 4, 2, 1, 2, 1) * 0.0625

in vec2 pass_Coordinates;

uniform sampler2D ColorTexture;
uniform sampler2D DepthTexture;

uniform bool Greyscale;
uniform bool Blur;
uniform bool Vertical;
uniform bool Horizontal;
uniform bool ChromaticAberration;

out vec4 out_Color;

vec2 resolution = vec2(640, 480);

vec4 computeGreyscale() {
    vec4 color_at_Frag = texture(ColorTexture, pass_Coordinates);
    vec3 luminance = vec3(0.2126, 0.7152, 0.0722);
    float grey_value = dot(color_at_Frag.rgb, luminance.rgb);
    vec4 final_Color;
    final_Color.rgb = mix(vec3(grey_value), color_at_Frag.rgb, vec3(0.0));
    final_Color.a = 1.0;

    return final_Color;
}

vec4 flipHorizontally() {
    vec2 flipped_Coordinates = vec2(1.0 - pass_Coordinates.s, 1.0 - pass_Coordinates.t);
    return texture(ColorTexture, flipped_Coordinates);
}

vec4 flipVertically() {
    vec2 flipped_Coordinates = vec2(pass_Coordinates.s, pass_Coordinates.t);
    return texture(ColorTexture, flipped_Coordinates);
}

vec4 computeBlur(vec2 uv, mat3 kernel) {
    vec4 average_Color = vec4(0.0);
    float direction[3] = {-1.0, 0.0, 1.0};
    for(int x_dir = 0; x_dir < 3; ++x_dir) {
        for (int y_dir = 0; y_dir < 3; ++y_dir) {
            vec2 offset = vec2(direction[x_dir], direction[y_dir]) / resolution.xy;
            average_Color += texture(ColorTexture, uv + offset) * kernel[x_dir][y_dir];
        }
    }
    return average_Color;
}

vec4 computeChromaticAberration() {
    float red_Offset   =  0.009;
    float green_Offset =  0.006;
    float blue_Offset  = -0.006;
    vec2 direction = pass_Coordinates - 0.5;

    vec4 final_Color;
    final_Color.r  = texture(ColorTexture, pass_Coordinates + (direction * vec2(red_Offset))).r;
    final_Color.g  = texture(ColorTexture, pass_Coordinates + (direction * vec2(green_Offset))).g;
    final_Color.ba = texture(ColorTexture, pass_Coordinates + (direction * vec2(blue_Offset))).ba;

    return final_Color;
}

void main() {
    if (Blur == true) {
        vec2 uv = pass_Coordinates;
        out_Color = computeBlur(uv, gaussian_blur);
    }
    else if (Vertical == true) {
        out_Color = flipVertically();
    }
    else if (Horizontal == true) {
        out_Color = flipHorizontally();
    }
    else if (ChromaticAberration == true){
        out_Color = computeChromaticAberration();
    }
    else if (Greyscale == true){
        out_Color = computeGreyscale();
    }

    out_Color = texture(ColorTexture, pass_Coordinates);
}