#version 150

uniform vec3  PlanetColor;
uniform vec3  AmbientColor;
uniform vec3  LightColor;
uniform float LightIntensity;
uniform vec3  LightPosition;
uniform bool  Cel;

const float Shininess = 50.0f;

in  vec3 pass_Position;
in  vec3 pass_Normal;
out vec4 out_Color;

void main() {
  vec3 LightDistance = LightPosition - pass_Position;
  float Distance = length(LightDistance);
  vec3 LightDirection = LightDistance / Distance;
  float DistSquared = Distance * Distance;
  LightDirection = normalize(LightDirection);

  vec3 ViewDirection = normalize(-pass_Position);

  float Lambertian = max(dot(LightDirection, pass_Normal), 0.0);
  vec3 HalfwayVector = normalize(LightDirection + ViewDirection);
  float SpecularAngle = max(dot(HalfwayVector, pass_Normal), 0.0);

  float SpecularCoefficient = pow(SpecularAngle, Shininess);
  float DiffuseCoefficient = 1.0;

  if (Cel) {
    SpecularCoefficient = round(SpecularCoefficient);
    Lambertian = ceil(2 * Lambertian) / 2;
  }

  vec3 Ambient = PlanetColor * AmbientColor;
  vec3 Diffuse = PlanetColor * Lambertian * LightColor * LightIntensity / Distance;
  vec3 Specular = vec3(1.0) * SpecularCoefficient * LightColor * LightIntensity / Distance;

  vec3 BlinnPhong = Ambient + Diffuse + Specular;

  out_Color =  vec4(BlinnPhong, 1.0);
}
