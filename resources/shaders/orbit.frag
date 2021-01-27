#version 150
in vec3 pass_Color;
out vec4 out_Color;

uniform vec3 ColorVertex;

void main () {
  out_Color = vec4(ColorVertex,1.0);
}

