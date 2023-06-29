#version 150
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_Coordinates;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 pass_Coordinates;

void main() {
    gl_Position = vec4(in_Position.x, in_Position.y, 0.0f, 1.0f);
    pass_Coordinates = in_Coordinates;

}