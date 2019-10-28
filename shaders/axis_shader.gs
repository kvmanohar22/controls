#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices=2) out;

in VS_OUT {
  vec3 col;
} gs_in[];

out GS_OUT {
  vec3 col;
} gs_out;

vec3 compute_triangle_normal(
  vec4 pt0, vec4 pt1, vec4 pt2)
{
  vec3 a = vec3(pt0) - vec3(pt1);  
  vec3 b = vec3(pt2) - vec3(pt1);  
  return normalize(cross(a, b));
}

void main() {
  vec4 pt0 = gl_in[0].gl_Position;
  vec4 pt1 = gl_in[1].gl_Position;
  vec4 pt2 = gl_in[2].gl_Position;

  vec3 normal = compute_triangle_normal(pt0, pt1, pt2);

  gs_out.col = gs_in[0].col;

  gl_Position = pt0;
  EmitVertex();  
  gl_Position = pt0 + 0.5*vec4(normal, 0.0f);
  EmitVertex();
  EndPrimitive();
}

