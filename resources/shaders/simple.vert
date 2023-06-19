#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Coordinates;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;
uniform vec4 CameraPosition; // position of the camera as derived from m_view_transform

out vec3 pass_Normal;
out vec4 pass_Position;
out vec4 pass_Camera;
out vec2 pass_Coordinates;

void main(void)
{
	pass_Position = ModelMatrix * vec4(in_Position, 1.0);
	pass_Camera = CameraPosition;
	gl_Position = (ProjectionMatrix * ViewMatrix) * pass_Position;
	pass_Normal = normalize(NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_Coordinates = in_Coordinates;

}
