#version 150
#extension GL_ARB_explicit_attrib_location : require
layout(location = 0) in vec3 in_Position;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ModelMatrix;

out vec3 pass_Coordinates;
out vec4 pass_Position;

void main() {
    pass_Position = (ProjectionMatrix * ViewMatrix) * vec4(in_Position, 1.0);
    gl_Position = vec4(pass_Position.x, pass_Position.y, pass_Position.w, pass_Position.w);
    //gl_Position = vec4(pass_Position.x, pass_Position.y, pass_Position.z, 1);
    pass_Coordinates = vec3(in_Position.x, in_Position.y, -in_Position.z);
}