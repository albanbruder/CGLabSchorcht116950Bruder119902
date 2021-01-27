#version 150

in  vec4 pass_TextureCoord;
out vec4 out_Color;

uniform sampler2D Texture;

void main() {
  vec2 texelCoord = vec2((atan(pass_TextureCoord.y, pass_TextureCoord.x) / 3.1415926 + 1.0) * 0.5,
                                (asin(pass_TextureCoord.z) / 3.1415926 + 0.5));

  out_Color = texture2D(Texture, texelCoord);
}