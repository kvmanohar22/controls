#include "controls/renderer.hpp"
#include "glm/gtx/string_cast.hpp"

namespace controls {

void PointRenderer::init() {
  float vertices[] = {
    // positions      // color
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  };

  /* TODO: Use instance buffer objects to render large number of objects */
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);
  glVertexAttribPointer(0, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void PointRenderer::render(Vector3d pos,
    Vector3d col,
    float size)
{
  // TODO: Better way to initialize model?
  glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
  model = glm::translate(model, glm::vec3(pos.x(), pos.y(), pos.z()));

  shader_->use();
  shader_->setmat4("model", model);
  shader_->setf("size", size);
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_POINTS, 0, 1);
  glBindVertexArray(0);
}

void AxisRenderer::init() {
  float vertices[] = {
    // triangle in yz-plane   // color
    0.0f, 0.0f, 0.0f,         1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,         1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,         1.0f, 0.0f, 0.0f,

    // triangle in xz-plane
    0.0f, 0.0f, 0.0f,         0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,         0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,         0.0f, 1.0f, 0.0f,
    
    // triangle in xy-plane
    0.0f, 0.0f, 0.0f,         0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,         0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,         0.0f, 0.0f, 1.0f,
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);
  glVertexAttribPointer(0, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void AxisRenderer::render() {
  // TODO: Better way to initialize model?
  glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

  shader_->use();
  shader_->setmat4("model", model);
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}

void CubeRenderer::init() {
  float vertices[] = {
    // vertices           // normal vectors
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    // -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //  0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    // -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    // -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    // -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    // -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    // -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    // -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    // -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    // -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    //  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //  0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    //  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    //  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    // -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    //  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    //  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    // -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    // -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    // -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    //  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    // -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    // -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);
  glVertexAttribPointer(0, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void CubeRenderer::render() {
  // TODO: Better way to initialize model?
  glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
  model = glm::scale(model, glm::vec3(2, 2, 2));

  shader_->use();
  shader_->setmat4("model", model);
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void CubeRenderer::render(Shader* new_shader) {
  // TODO: Better way to initialize model?
  glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
  model = glm::scale(model, glm::vec3(2, 2, 2));

  new_shader->use();
  new_shader->setmat4("model", model);
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}


} // namespace controls
