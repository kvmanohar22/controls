#include "controls/renderer.hpp"
#include "glm/gtx/string_cast.hpp"

namespace controls {

void Renderer::init() {
  float vertices[] = {
    // positions      // color
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
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

void Renderer::render() {
  // TODO: Implement
}

} // namespace controls
