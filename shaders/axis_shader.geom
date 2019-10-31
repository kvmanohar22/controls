#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices=2) out;

in VS_OUT {
  vec3 normal;
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

void render_normal(vec4 pt, vec3 normal) {
  gl_Position = pt;
  EmitVertex();

  gl_Position = pt + 2.2f * vec4(normalize(normal), 0.0f);
  EmitVertex();

  EndPrimitive();
}

void render_normals_manually() {
  vec4 pt0 = gl_in[0].gl_Position;
  vec4 pt1 = gl_in[1].gl_Position;
  vec4 pt2 = gl_in[2].gl_Position;

  vec3 normal = compute_triangle_normal(pt0, pt1, pt2);

  gs_out.col = vec3(1.0f, 0.0f, 0.0f);
  render_normal(pt0, normal);
  render_normal(pt1, normal);
  render_normal(pt2, normal);
}

void render_normals_normal_vector() {
  vec4 pt0 = gl_in[0].gl_Position;
  vec4 pt1 = gl_in[1].gl_Position;
  vec4 pt2 = gl_in[2].gl_Position;

  // WARNING: Colors are treated as normals
  //          When colors are passed, be careful
  vec3 normal = gs_in[0].normal;
  vec3 color = gs_in[0].col;

  gs_out.col = color;
  render_normal(pt0, normal);
  // render_normal(pt1, normal);
  // render_normal(pt2, normal);
}

void main() {
  // render_normals_manually();
  render_normals_normal_vector();
}
