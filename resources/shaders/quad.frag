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

vec4 computeGreyscale(vec4 color_at_Frag) {
    vec3 luminance = vec3(0.2126, 0.7152, 0.0722);
    float grey_value = dot(color_at_Frag.rgb, luminance.rgb);
    // returns mix between grey value of color and current color at this fragment, with a "intensity" value for the greyscale effect, and 1 for full opacity
    return vec4(mix(vec3(grey_value), color_at_Frag.rgb, vec3(0.0)), 1.0);
}

// flip coordinates on the respective axes
vec2 flipHorizontally(vec2 coordinates) {
    return vec2(1.0 - coordinates.s, coordinates.t);
}

vec2 flipVertically(vec2 coordinates) {
    return vec2(coordinates.s, 1.0 - coordinates.t);
}

// consider 3x3 field around current pixel and add together to an average color at that pixel, can be done with any cube blur
vec4 computeBlur(vec2 coordinates, mat3 kernel) {
    vec4 average_Color = vec4(0.0);
    float direction[3] = {-1.0, 0.0, 1.0};
    for(int x_dir = 0; x_dir < 3; ++x_dir) {
        for (int y_dir = 0; y_dir < 3; ++y_dir) {
            vec2 offset = vec2(direction[x_dir], direction[y_dir]) / resolution.xy;
            average_Color += texture(ColorTexture, coordinates + offset) * kernel[x_dir][y_dir];
        }
    }
    return average_Color;
}

// put small offset into three color channels individually, only in one direction for ease's sake
vec4 computeChromaticAberration(vec2 coordinates) {
    float red_Offset   =  0.009;
    float green_Offset =  0.006;
    float blue_Offset  = -0.006;
    vec2 direction = coordinates - 0.5;

    vec4 final_Color;
    final_Color.r  = texture(ColorTexture, coordinates + (direction * vec2(red_Offset))).r;
    final_Color.g  = texture(ColorTexture, coordinates + (direction * vec2(green_Offset))).g;
    final_Color.ba = texture(ColorTexture, coordinates + (direction * vec2(blue_Offset))).ba;

    return final_Color;
}

void main() {
    vec2 coordinates = pass_Coordinates;

    if (Vertical) {
        coordinates = flipVertically(coordinates);
    }
    if (Horizontal) {
        coordinates = flipHorizontally(coordinates);
    }

    if (Blur) {
        out_Color = computeBlur(coordinates, gaussian_blur);
    } else if (ChromaticAberration) {
        out_Color = computeChromaticAberration(coordinates);
    } else {
        out_Color = texture(ColorTexture, coordinates);
    }


    if (Greyscale) {
        out_Color = computeGreyscale(out_Color);
    }



}