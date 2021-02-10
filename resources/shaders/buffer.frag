#version 150 core

in vec2 pass_TexCoords;
out vec4 out_Color;

uniform sampler2D screenTexture;

void main()
{
    out_Color = texture(screenTexture, pass_TexCoords);
}
