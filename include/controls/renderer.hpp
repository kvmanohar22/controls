#ifndef _CONTROLS_RENDERER_H_
#define _CONTROLS_RENDERER_H_

#include "controls/global.hpp"
#include "controls/shader.hpp"

namespace controls {

/* Different types of vertex data */
enum class VertexDataType {
  VERTEX,
  VERTEX_COLOR,
  VERTEX_COLOR_TEXTURE,
  VERTEX_COLOR_TEXTURE_NORMAL
};

/* Abstract Renderer 
 * 
 * All classes should derive this and implement init()
 *
 * */
class Renderer {
public:
  Shader* shader_;
  GLuint  VAO_;

  Renderer(Shader* shader)
    :  shader_(shader)
  {}
 ~Renderer() {
    delete shader_;
  }

  virtual void init() =0;
};

class PointRenderer : public Renderer {
public:
  PointRenderer(Shader* shader)
    : Renderer(shader)
  {
    init(); 
  }
 ~PointRenderer() {}

  void init();
  void render(Vector3d pos, 
      Vector3d color,
      float size);

private:
  vector<float>  vertex_data_;
  VertexDataType vertex_data_type_;
};

class AxisRenderer : public Renderer {
public:
  AxisRenderer(Shader* shader)
    : Renderer(shader)
  {
    init(); 
  }
 ~AxisRenderer() {}

  void init();
  void render();
};

class CubeRenderer : public Renderer {
public:
  CubeRenderer(Shader* shader)
    : Renderer(shader)
  {
    init(); 
  }
 ~CubeRenderer() {}

  void init();
  void render();                   // render the same old cube
  void render(Shader* new_shader); // render the normals
};

} // namespace controls

#endif
