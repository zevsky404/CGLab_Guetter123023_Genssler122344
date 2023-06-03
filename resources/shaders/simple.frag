#version 150

uniform vec3  PlanetColor;
uniform vec3  AmbientColor;
uniform vec3  LightColor;
uniform float LightIntensity;
uniform vec3  LightPosition;

const float Shininess = 16.0f;

in  vec3 pass_Position;
in  vec3 pass_Normal;
out vec4 out_Color;

void main() {
  vec3 LightDirection = LightPosition - pass_Position;
  float Distance = length(LightDirection);

  vec3 ViewDirection = normalize(-pass_Position);

  float Lambertian = max(dot(LightDirection, pass_Normal), 0.0);
  vec3 HalfwayVector = normalize(LightDirection + ViewDirection);
  float SpecularAngle = max(dot(HalfwayVector, pass_Normal), 0.0);

  float SpecularCoefficient = pow(SpecularAngle, Shininess);
  float DiffuseCoefficient = 0.1;

  vec3 BlinnPhong = PlanetColor * AmbientColor +
                      PlanetColor * Lambertian * LightColor * LightIntensity / Distance +
                      vec3(1.0) * SpecularCoefficient * LightColor * LightIntensity / Distance;

  out_Color = vec4(BlinnPhong, 1.0);
}
