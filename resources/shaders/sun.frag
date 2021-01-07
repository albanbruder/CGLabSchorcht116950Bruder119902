#version 150

in  vec3 pass_Normal;
out vec4 out_Color;

void main() {
  out_Color = vec4(vec3(1.0, 1.0, 0.1) * 0.8  + abs(normalize(pass_Normal)) * 0.2, 1.0);
}