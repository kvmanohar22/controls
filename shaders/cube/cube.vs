#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 icol;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT {
  vec3 col;
} vs_out;

void main() {
  gl_Position = projection * view * model * vec4(ipos, 1.0f);
  // vs_out.col = icol;
  vs_out.col = vec3(0.0f, 0.0f, 0.0f);
}
