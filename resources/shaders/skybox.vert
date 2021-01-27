#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 in_Position;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 pass_TextureCoord;

void main()
{
    pass_TextureCoord = in_Position;
    gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
}  