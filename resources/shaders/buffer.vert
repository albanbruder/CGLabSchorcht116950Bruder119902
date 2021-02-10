#version 150 core
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec2 in_Position;
out vec2 pass_TexCoords;

void main()
{
    gl_Position = vec4(in_Position, 0.0, 1.0);
    pass_TexCoords = vec2(in_Position.x/2+0.5, in_Position.y/2+0.5);
}
