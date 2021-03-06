#version 150

in  vec3 WorldSpaceNormal;
in vec3 WorldSpacePosition;
in  vec4 pass_TextureCoord;
out vec4 out_Color;

uniform float LightIntensity;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 CameraPosition;
uniform sampler2D Texture;

void main() {
  // calculate texel coordinates
  float pi = 3.1415926;
  float u = (atan(-pass_TextureCoord.z, pass_TextureCoord.x) / pi + 1.0) * 0.5;
  float v = asin(-pass_TextureCoord.y) / pi + 0.5;
  vec2 texelCoord = vec2(u, v);

  vec4 color = texture2D(Texture, texelCoord);
  
  // ambient component
  float ambientStrength = 0.1;
  vec3 ambientColor = LightColor * ambientStrength;

  // diffuse component
  vec3 normal = normalize(WorldSpaceNormal);
  vec3 lightDirection = normalize(LightPosition - WorldSpacePosition);
  float diffuseStrength = max(dot(normal, lightDirection), 0.0);
  vec3 diffuseColor = diffuseStrength * LightColor;

  // specular component
  float specularStrength = 0.5;
  float specularExponent = 128;
  vec3 viewDir = normalize(CameraPosition - WorldSpacePosition);
  vec3 halfDir = normalize(lightDirection + viewDir);
  float spec = pow(max(dot(halfDir, normal), 0.0), specularExponent);
  vec3 specularColor = LightColor * specularStrength * spec;

  vec3 combinedColor = LightIntensity * (ambientColor + diffuseColor + specularColor) * vec3(color.x, color.y, color.z);

  out_Color = vec4(combinedColor, 1.0);
}
