#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 icol;
layout (location = 2) in vec3 ioffset;

out VS_OUT {
  vec3 col;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float size;
uniform vec3 color;

void main() {
  vs_out.col = color;
  gl_Position = projection * view * model * vec4(ipos+ioffset, 1.0f);
  gl_PointSize = 5.0f;
}
