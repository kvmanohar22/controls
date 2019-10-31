#version 330 core
out vec4 frag_color;

in VS_OUT {
  vec3 col;
} fs_in;

void main() {
  frag_color = vec4(fs_in.col, 1.0);
}
