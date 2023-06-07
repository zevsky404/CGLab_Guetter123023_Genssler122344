#version 150

#define PI 3.1415926538



uniform vec3  PlanetColor; // colour of the planet
uniform vec3  AmbientColor; // colour of the ambient light
uniform vec3  LightColor;  // colour of the point light
uniform float LightIntensity; // intensity of the point light
uniform vec3  LightPosition; // position of the point light
uniform bool  Cel; // bool for actvating cel shading

const float Shininess = 10.0f;

in  vec4 pass_Position;
in  vec3 pass_Normal;
in  vec4 pass_Camera;
out vec4 out_Color;

void main() {
  vec3 LightDistance = LightPosition - pass_Position.xyz;

  float Distance = length(LightDistance);
  vec3 LightDirection = normalize(LightDistance);
  float DistSquared = Distance * Distance;
  vec3 ViewDirection = normalize(pass_Camera.xyz - pass_Position.xyz);

  float Lambertian = max(dot(LightDirection, pass_Normal), 0.0);

  vec3 HalfwayVector = normalize(LightDirection + ViewDirection);
  float SpecularAngle = max(dot(pass_Normal, HalfwayVector), 0.0);
  float SpecularCoefficient = pow(SpecularAngle, Shininess);

  if (Cel) {
    SpecularCoefficient = round(SpecularCoefficient);
    Lambertian = ceil(2 * Lambertian) / 2;
  }

  vec3 Ambient = PlanetColor * AmbientColor * 1.5f;
  vec3 Diffuse = PlanetColor * Lambertian * LightColor * LightIntensity / Distance;
  vec3 Specular = vec3(1.0) * SpecularCoefficient * LightColor * LightIntensity / Distance;

  vec3 BlinnPhong = Ambient + Diffuse + Specular;

  out_Color =  vec4(BlinnPhong, 1.0);
}
