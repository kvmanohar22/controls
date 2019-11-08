#version 330 core
layout (location = 0) in vec3 ipos;

out VS_OUT {
  vec3 col;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform vec3 color;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(ipos, 1.0f);
  vs_out.col = color;
}
