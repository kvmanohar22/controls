#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 icol;

out VS_OUT {
  vec3 col;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform float size;
uniform mat4 projection;

void main() {
  vs_out.col = icol;
  gl_Position = projection * view * model * vec4(ipos, 1.0f);
  gl_PointSize = size;
}
