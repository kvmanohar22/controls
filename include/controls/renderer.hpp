#ifndef _CONTROLS_RENDERER_H_
#define _CONTROLS_RENDERER_H_

#include "controls/global.hpp"
#include "controls/shader.hpp"
#include "controls/config.hpp"
#include "controls/linear_continuous_system.hpp"

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
    instance_data_.resize(0xF0000); 
    std::fill(instance_data_.begin(), instance_data_.end(), 2.0f); 
  }
 ~PointRenderer() {}

  void init();
  void render(Vector3d pos, 
      Vector3d color,
      float size);
  void render(vector<Vector3d> pos, 
      Vector3d color,
      float size);
  void render(PARTICLE_TRAIL pos);
  void render_instanced_buffer(size_t n_instances);

  // recreate the buffer
  size_t resize_buffer(PARTICLE_TRAIL pos);

private:
  vector<float>  vertex_data_;
  VertexDataType vertex_data_type_;
  unsigned int   instance_VBO_;
  vector<float>  instance_data_;
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

  // a new shader provided? => render with old + this new
  // TODO: Could be done in a better way 
  void render();
};

} // namespace controls

#endif
