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
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat2x4), NULL, GL_STREAM_DRAW); 
  glVertexAttribPointer(2, 4,
      GL_FLOAT, GL_FALSE, 4*sizeof(glm::vec4), (void*)0);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 4,
      GL_FLOAT, GL_FALSE, 4*sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glBindVertexArray(0);
}

size_t PointRenderer::resize_buffer(PARTICLE_TRAIL pos) {
  size_t n_particles = 0;
  for(size_t i=0; i<pos.size(); ++i)
    n_particles += (pos[i].second.size()+1);

  size_t n_particles_so_far = 0;
  // std::fill(instance_data_.begin(), instance_data_.end(), 2.0f);
  for(size_t p=0; p<pos.size(); ++p) {
    const size_t n_new_particles = pos[p].second.size();
    Particle* parent = pos[p].first;
    list<Particle*> trail = pos[p].second;

    // add position and size data
    instance_data_[n_particles_so_far+0] = parent->x_.x();
    instance_data_[n_particles_so_far+1] = parent->x_.y();
    instance_data_[n_particles_so_far+2] = parent->x_.z();
    instance_data_[n_particles_so_far+3] = 5.0f;           // change the size of parent
    // add color data
    instance_data_[n_particles_so_far+4] = parent->col_.x();
    instance_data_[n_particles_so_far+5] = parent->col_.y();
    instance_data_[n_particles_so_far+6] = parent->col_.z();
    instance_data_[n_particles_so_far+7] = 1.0f;
    
    list<Particle*>::iterator itr=trail.begin();
    for(size_t i=1; itr!=trail.end(); ++itr, ++i) {
      instance_data_[n_particles_so_far+i*8+0] = (*itr)->x_.x();
      instance_data_[n_particles_so_far+i*8+1] = (*itr)->x_.y();
      instance_data_[n_particles_so_far+i*8+2] = (*itr)->x_.z();
      instance_data_[n_particles_so_far+i*8+3] = 2.0f;

      instance_data_[n_particles_so_far+i*8+4] = (*itr)->col_.x();
      instance_data_[n_particles_so_far+i*8+5] = (*itr)->col_.y();
      instance_data_[n_particles_so_far+i*8+6] = (*itr)->col_.z();
      instance_data_[n_particles_so_far+i*8+7] = (*itr)->life_;
    }
    n_particles_so_far += ((n_new_particles+1)*8);
  }
  glBindBuffer(GL_ARRAY_BUFFER, instance_VBO_);
  glBufferData(GL_ARRAY_BUFFER,
      n_particles*sizeof(glm::mat2x4), instance_data_.data(), GL_STREAM_DRAW); 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return n_particles;
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

void PointRenderer::render_instanced_buffer(size_t n_instances)
{
  glBindVertexArray(VAO_);
  shader_->use();
  glm::mat4 model = glm::mat4(1.0f);
  shader_->setmat4("model", model);
  glDrawArraysInstanced(GL_POINTS, 0, 1, n_instances);
  glBindVertexArray(0);
}

void PointRenderer::render(PARTICLE_TRAIL pos) {
  // Resize the buffer to accomodate new particles
  const size_t n_particles = resize_buffer(pos);

  // Render the entire thing in one call
  render_instanced_buffer(n_particles); 
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
  glLineWidth(9.0f);

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
  shader_->use();
  shader_->setvec3("color", glm::vec3(color_.x(), color_.y(), color_.z()));
}

void CubeRenderer::render() {
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(10));
  glLineWidth(9.0f);

  shader_->use();
  shader_->setmat4("model", model);
  shader_->setvec3("color", glm::vec3(color_.x(), color_.y(), color_.z()));
  glBindVertexArray(VAO_);
  glDrawArrays(GL_LINES, 0, 24);
  glBindVertexArray(0);
}

void PlaneRenderer::init() {
  size_t n_lines = 50;
  float x_min = -100;
  float x_max =  100;
  float y_min = -100;
  float y_max =  100;
  float dx = (x_max-x_min) / n_lines;
  float dy = (y_max-y_min) / n_lines;

  vector<float> lines;
  lines.reserve(2*6*n_lines);
  // lines parallel to y-axis 
  for(size_t i=0; i<n_lines; ++i) {
    float xcurr = x_min + dx * i; 
    lines.push_back(xcurr); 
    lines.push_back(0.0f); 
    lines.push_back(y_min); 

    lines.push_back(xcurr); 
    lines.push_back(0.0f); 
    lines.push_back(y_max); 
  }
  // lines parallel to x-axis 
  for(size_t i=0; i<n_lines; ++i) {
    float ycurr = y_min + dy * i; 
    lines.push_back(x_min); 
    lines.push_back(0.0f); 
    lines.push_back(ycurr); 

    lines.push_back(x_max); 
    lines.push_back(0.0f); 
    lines.push_back(ycurr); 
  }
  n_lines_ = 2*2*n_lines;
  
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, lines.size()*sizeof(float), lines.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);
  glVertexAttribPointer(0, 3,
      GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  shader_->use();
  shader_->setvec3("color", glm::vec3(color_.x(), color_.y(), color_.z()));
  glm::mat4 model = glm::mat4(1.0f);
  shader_->setmat4("model", model);
}

void PlaneRenderer::render() {
  glLineWidth(1.0f);
  shader_->use();
  shader_->setmat4("model", glm::mat4(1.0f));
  shader_->setvec3("color", glm::vec3(color_.x(), color_.y(), color_.z()));
  glBindVertexArray(VAO_);
  glDrawArrays(GL_LINES, 0, n_lines_);
  glBindVertexArray(0);
}

} // namespace controls
