#version 330 core
out vec4 frag_color;

in VS_OUT {
  vec2 tex;
} fs_in;

uniform sampler2D image;

void main() {
  frag_color = texture(image, fs_in.tex);
}
