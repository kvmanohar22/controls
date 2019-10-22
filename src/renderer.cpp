#include "controls/renderer.hpp"
#include "glm/gtx/string_cast.hpp"

namespace controls {

void Renderer::init() {
  float vertices[] = {
    // positions      // color
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::render(Vector3d pos,
    float size,
    Vector3d col)
{
  glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
  model = glm::translate(model, glm::vec3(pos.x(), pos.y(), pos.z()));

  shader_->use();
  shader_->setmat4("model", model);
  shader_->setvec3("color", glm::vec3(col.x(), col.y(), col.z()));
  shader_->setf("size", size);
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_POINTS, 0, 1);
  glBindVertexArray(0);
}

} // namespace controls
