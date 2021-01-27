#version 150

in  vec4 pass_TextureCoord;
out vec4 out_Color;

uniform sampler2D Texture;

void main() {
  // calculate texel coordinates
  float pi = 3.1415926;
  float u = (atan(-pass_TextureCoord.z, pass_TextureCoord.x) / pi + 1.0) * 0.5;
  float v = asin(-pass_TextureCoord.y) / pi + 0.5;
  vec2 texelCoord = vec2(u, v);

  out_Color = texture2D(Texture, texelCoord);
}