#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec2 itex;

out VS_OUT {
  vec2 tex;
} vs_out;

uniform mat4 model;
uniform mat4 projection;

void main() {
  vs_out.tex = itex;
  gl_Position = projection * model * vec4(ipos, 1.0f);
}
