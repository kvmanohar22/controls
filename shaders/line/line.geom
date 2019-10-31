#version 330 core
layout (lines) in;
layout (line_strip, max_vertices=2) out;

in VS_OUT {
  vec3 col;
} gs_in[];

out GS_OUT {
  vec3 col;
} gs_out;

void main() {
  gs_out.col = gs_in[0].col;
  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  EmitVertex();

  EndPrimitive();
}
