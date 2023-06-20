#version 150

uniform vec3  PlanetColor;     // color of the planet
uniform vec3  AmbientColor;    //color of the ambient light
uniform vec3  LightColor;      // color of the point light
uniform float LightIntensity;  // intensity of the point light
uniform vec3  LightPosition;   // position of the point light
uniform bool  Cel;             // bool for activating cel shading
uniform sampler2D TextureSampler;

const float Shininess = 10.0;  // specular exponent to determine shininess
vec3 TextureColor;

in  vec4 pass_Position;
in  vec3 pass_Normal;
in  vec4 pass_Camera;
in  vec2 pass_Coordinates;
out vec4 out_Color;

void main() {
  vec3 Normal = normalize(pass_Normal);
  vec3 LightDistance = LightPosition - pass_Position.xyz;                   // unnormalised distance between point light and geometry

  vec3 LightDirection = normalize(LightDistance);                           // direction of the light in respect to the geometry
  float Distance = length(LightDistance);                                   // normalised distance
  vec3 ViewDirection = normalize(pass_Camera.xyz - pass_Position.xyz);      // direction of the camera in respect to the geometry

  float DiffuseFactor = max(dot(Normal, LightDirection), 0.0);              // coefficient to determine diffuseness

  vec3 ReflectionDirection = reflect(-LightDirection, Normal);
  float SpecularAngle = max(dot(ReflectionDirection, ViewDirection), 0.0);  // angle of specular reflection
  float SpecularCoefficient = pow(SpecularAngle, Shininess);                // specular coefficient to determine strength and angle

  if (Cel) {                                                                // if cel shading is active, coefficients are capped to get rid of smooth fall-off
    SpecularCoefficient = round(SpecularCoefficient);
    DiffuseFactor = ceil(2 * DiffuseFactor) / 2;
  }

  TextureColor = texture(TextureSampler, pass_Coordinates).xyz;
  // caluclation of three different light components
  vec3 Ambient = AmbientColor * TextureColor * 0.3;
  vec3 Diffuse = LightIntensity * LightColor * TextureColor * DiffuseFactor / Distance;
  vec3 Specular = LightIntensity * LightColor * SpecularCoefficient / Distance;

  vec3 BlinnPhong = Ambient + Diffuse + Specular;

  out_Color = vec4(BlinnPhong, 1.0);
}