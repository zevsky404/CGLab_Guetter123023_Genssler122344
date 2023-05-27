#version 150

uniform vec3 PlanetColor;
// uniform vec3 ambient_Color;

in  vec3 pass_Normal;
out vec4 out_Color;

void main() {
  //out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
  out_Color = vec4(PlanetColor, 1.0);
}
