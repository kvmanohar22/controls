#version 330 core
layout (location = 0) in vec3 ipos;
layout (location = 1) in vec3 icol;
layout (location = 2) in vec3 inormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT {
  vec3 normal;
  vec3 col;
} vs_out;

void main() {
  gl_Position = projection * view * model * vec4(ipos, 1.0f);
  mat3 normal_matrix = mat3(transpose(inverse(view * model)));
  vs_out.normal = vec3(projection * vec4(normal_matrix * icol, 0.0f));
  vs_out.col = icol;
}
