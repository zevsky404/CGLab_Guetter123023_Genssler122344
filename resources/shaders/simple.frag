#version 150

#define PI 3.1415926538



uniform vec3  PlanetColor; // colour of the planet
uniform vec3  AmbientColor; // colour of the ambient light
uniform vec3  LightColor;  // colour of the point light
uniform float LightIntensity; // intensity of the point light
uniform vec3  LightPosition; // position of the point light
uniform bool  Cel; // bool for actvating cel shading

const float Shininess = 10.0f; // specular exponent to determine shininess

in  vec4 pass_Position;
in  vec3 pass_Normal;
in  vec4 pass_Camera;
out vec4 out_Color;

void main() {
  vec3 LightDistance = LightPosition - pass_Position.xyz; // unnormalised distance between point light and geometry

  float Distance = length(LightDistance);  // normalised distance
  vec3 LightDirection = normalize(LightDistance); // direction of the light in respect to the geometry
  float DistSquared = Distance * Distance;
  vec3 ViewDirection = normalize(pass_Camera.xyz - pass_Position.xyz); // direction of the camera in respect to the geometry

  float Lambertian = max(dot(LightDirection, pass_Normal), 0.0);  // coefficient to determine diffuseness

  vec3 HalfwayVector = normalize(LightDirection + ViewDirection);  // helper vector to make specular reflection easier to calculate
  float SpecularAngle = max(dot(pass_Normal, HalfwayVector), 0.0);  // angle of specular reflection
  float SpecularCoefficient = pow(SpecularAngle, Shininess);  // specular coefficient to determine strength and angle

  if (Cel) {  // if cel shading is active, coefficients are capped to get rid of smooth fall-off
    SpecularCoefficient = round(SpecularCoefficient);
    Lambertian = ceil(2 * Lambertian) / 2;
  }

  // caluclation of three different light components
  vec3 Ambient = PlanetColor * AmbientColor * 1.5f;
  vec3 Diffuse = PlanetColor * Lambertian * LightColor * LightIntensity / Distance;
  vec3 Specular = PlanetColor * SpecularCoefficient * LightColor * LightIntensity / Distance;

  vec3 BlinnPhong = Ambient + Diffuse + Specular;

  out_Color =  vec4(BlinnPhong, 1.0);
}
