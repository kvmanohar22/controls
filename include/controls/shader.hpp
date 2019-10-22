#ifndef _CONTROLS_SHADER_H_
#define _CONTROLS_SHADER_H_

#include "controls/global.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

namespace controls {

class Shader {
public:
  unsigned int id_;

  Shader() {}
  Shader(const char* v_path, const char* f_path);

  Shader* use();

  void seti(const char *name, const int val) const;
  void setf(const char *name, const float val) const;
  void setmat4(const char *name, const glm::mat4 val) const;
  void setvec2(const char *name, const glm::vec2 val) const;
  void setvec3(const char *name, const glm::vec3 val) const;
  void setuniform(const char *name, const int binding_point) const;
}; // class Shader

} // namespace controls

#endif
