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
  shader_->setvec3("color", glm::vec3(col.x(), col.y(), col.z()));
  shader_->setf("size", size);
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_POINTS, 0, 1);
  glBindVertexArray(0);
}

void PointRenderer::render(PARTICLE_TRAIL pos) {
  glBindVertexArray(VAO_);
  shader_->use();
  for(size_t i=0; i<pos.size(); ++i) {
    glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    
    // Render the parent
    const Particle* parent = pos[i].first;
    render(parent->x_, parent->col_, 3.0f);

    // Render the tail of the parent
    list<Particle*> trail = pos[i].second;
    std::for_each(trail.begin(), trail.end(), [&](Particle* tail) {
      render(tail->x_, tail->col_, 1.0f);
    });
  }
  glBindVertexArray(0);
}


void PointRenderer::render(vector<Vector3d> pos,
    Vector3d col, float size)
{
  glBindVertexArray(VAO_);
  shader_->use();
  shader_->setf("size", size);
  for(size_t i=0; i<pos.size(); ++i) {
    glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    model = glm::translate(model, glm::vec3(pos[i].x(), pos[i].y(), pos[i].z()));

    shader_->setmat4("model", model);
    glDrawArrays(GL_POINTS, 0, 1);
  }
  glBindVertexArray(0);
}

void AxisRenderer::init() {
  float vertices[] = {
    // triangle in yz-plane   // normal
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
  glDrawArrays(GL_TRIANGLES, 0, 9);
  glBindVertexArray(0);
}

void CubeRenderer::init() {
  float vertices[] = {
    // vertices
      1.0f,  1.0f,  1.0f,
     -1.0f,  1.0f,  1.0f,

      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f, -1.0f,

      1.0f,  1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,

     -1.0f,  1.0f,  1.0f,
     -1.0f, -1.0f,  1.0f,

     -1.0f,  1.0f,  1.0f,
     -1.0f,  1.0f, -1.0f,

     -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,

     -1.0f, -1.0f,  1.0f,
     -1.0f, -1.0f, -1.0f,

      1.0f, -1.0f,  1.0f,
      1.0f, -1.0f, -1.0f,

     -1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,

      1.0f,  1.0f, -1.0f,
     -1.0f,  1.0f, -1.0f,

      1.0f,  1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,

     -1.0f,  1.0f, -1.0f,
     -1.0f, -1.0f, -1.0f
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);
  glVertexAttribPointer(0, 3,
      GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void CubeRenderer::render() {
  // TODO: Better way to initialize model?
  glm::mat4 model = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
  model = glm::scale(model, glm::vec3(10, 10, 10));

  shader_->use();
  shader_->setmat4("model", model);
  glBindVertexArray(VAO_);
  glDrawArrays(GL_LINES, 0, 24);
  glBindVertexArray(0);
}

} // namespace controls
