#version 150

in  vec3 WorldSpaceNormal;
in vec3 WorldSpacePosition;
out vec4 out_Color;

uniform vec3 ColorVertex;
uniform vec3 CameraPosition;

void main() {

  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightPos = vec3(0.0, -10.0, 0.0);

  float ambientStrength = 0.1;
  vec3 ambientColor = ambientStrength * lightColor;

  vec3 norm = normalize(WorldSpaceNormal);
  vec3 lightDir = normalize(lightPos - WorldSpacePosition);
  float diffuseStrength = max(dot(norm, lightDir), 0.0);
  vec3 diffuseColor = diffuseStrength * lightColor;

  float specularStrength = 0.5;
  vec3 viewDir = normalize(CameraPosition - WorldSpacePosition);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
  vec3 specularColor = specularStrength * spec * lightColor;

  vec3 combinedColor = (ambientColor + diffuseColor + specularColor) * ColorVertex;

  out_Color = vec4(combinedColor, 1.0);
}
