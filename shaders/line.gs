#version 330 core
layout (points) in;
layout (line_strip, max_vertices=3) out;

in VS_OUT {
  vec3 col;
} gs_in[];

out VS_OUT {
  vec3 col;
} gs_out;

void main() {
  gs_out.col = gs_in[0].col;
  vec4 position = gl_in[0].gl_Position;

  gl_Position = position + vec4(3, 0, 0, 0.0f);
  EmitVertex();   
 
  // gl_Position = position + vec4(0, 3, 0, 0.0f);
  // EmitVertex();   
 
  // gl_Position = position + vec4(0, 0, 3, 0.0f);
  // EmitVertex();   

  EndPrimitive();
}

