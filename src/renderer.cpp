#include "controls/renderer.hpp"
#include "glm/gtx/string_cast.hpp"

namespace controls {

void PointRenderer::init() {
  float vertices[] = {
    // positions      // color
    0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  };

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // normal buffer for position and texture
  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);
  glVertexAttribPointer(0, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3,
      GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // instanced buffer for offset *changes dynamically*
  glGenBuffers(1, &instance_VBO_);
  glBindBuffer(GL_ARRAY_BUFFER, instance_VBO_);
  glBufferData(GL_ARRAY_BUFFER, 4*sizeof(float), NULL, GL_STREAM_DRAW); 
  glVertexAttribPointer(2, 4,
      GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glVertexAttribDivisor(2, 1);

  glBindVertexArray(0);
}

void PointRenderer::resize_buffer(const Particle* parent,
    list<Particle*>& new_particles)
{
  const size_t n_particles = new_particles.size();
  // offset
  vector<float> raw_pose(n_particles*4+4, 2.0f);
  raw_pose[0] = parent->x_.x();
  raw_pose[1] = parent->x_.y();
  raw_pose[2] = parent->x_.z();
  raw_pose[3] = 5.0f;           // change the size of parent
  list<Particle*>::iterator itr=new_particles.begin();
  for(size_t i=1; itr!=new_particles.end(); ++itr, ++i) {
    raw_pose[i*4+0] = (*itr)->x_.x();
    raw_pose[i*4+1] = (*itr)->x_.y();
    raw_pose[i*4+2] = (*itr)->x_.z();
  }
  glBindBuffer(GL_ARRAY_BUFFER, instance_VBO_);
  glBufferData(GL_ARRAY_BUFFER,
      (n_particles+1)*4*sizeof(float), raw_pose.data(), GL_STREAM_DRAW); 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  raw_pose.clear(); 
}

void PointRenderer::render(Vector3d pos,
    Vector3d col, float size)
{
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(pos.x(), pos.y(), pos.z()));

  shader_->use();
  shader_->setmat4("model", model);
  shader_->setvec3("color", glm::vec3(col.x(), col.y(), col.z()));
  
  glBindVertexArray(VAO_);
  glDrawArrays(GL_POINTS, 0, 1);
  glBindVertexArray(0);
}

void PointRenderer::render_instanced_buffer(size_t n_instances,
    const Vector3d& c)
{
  // TODO: Point sizes to be changed 
  glBindVertexArray(VAO_);
  shader_->use();
  glm::mat4 model = glm::mat4(1.0f);
  shader_->setmat4("model", model);
  shader_->setvec3("color", glm::vec3(c.x(), c.y(), c.z()));
  glDrawArraysInstanced(GL_POINTS, 0, 1, n_instances);
  glBindVertexArray(0);
}

void PointRenderer::render(PARTICLE_TRAIL pos) {
  for(size_t i=0; i<pos.size(); ++i) {
    // Render the parent and it's children
    const Particle* parent = pos[i].first;
    list<Particle*> trail = pos[i].second;

    // Resize the buffer to accomodate new particles
    resize_buffer(parent, trail);

    // Render the entire thing in one call
    render_instanced_buffer(trail.size()+1, parent->col_); 
  }
}

void PointRenderer::render(vector<Vector3d> pos,
    Vector3d col, float size)
{
  glBindVertexArray(VAO_);
  shader_->use();
  shader_->setf("size", size);
  for(size_t i=0; i<pos.size(); ++i) {
    glm::mat4 model = glm::mat4(1.0f);
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
  glm::mat4 model = glm::mat4(1.0f);

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
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(10, 10, 10));

  shader_->use();
  shader_->setmat4("model", model);
  glBindVertexArray(VAO_);
  glDrawArrays(GL_LINES, 0, 24);
  glBindVertexArray(0);
}

} // namespace controls
