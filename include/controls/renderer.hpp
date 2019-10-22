#ifndef _CONTROLS_RENDERER_H_
#define _CONTROLS_RENDERER_H_

#include "controls/global.hpp"
#include "controls/shader.hpp"

namespace controls {

class Renderer {
public:
  Shader* shader_;
  GLuint  VAO_;

  Renderer(Shader* shader)
    :  shader_(shader)
  {
    init();
  }
 ~Renderer();

  void init();
  void render();
};

} // namespace controls

#endif
