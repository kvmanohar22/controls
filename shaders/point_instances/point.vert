#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 icol;
layout (location = 2) in mat2x4 ioffset;

out VS_OUT {
  vec4 col;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

void main() {
  vs_out.col = ioffset[1].xyzw;
  gl_Position = projection * view * model * vec4(ipos+ioffset[0].xyz, 1.0f);
  gl_PointSize = ioffset[0].w;
}
