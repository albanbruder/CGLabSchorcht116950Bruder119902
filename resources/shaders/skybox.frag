#version 150

in vec3 pass_TextureCoord;
out vec4 out_Color;

uniform samplerCube Texture;

void main()
{    
    out_Color = texture(Texture, pass_TextureCoord);
}