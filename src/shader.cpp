#include "controls/shader.hpp"

namespace controls {

Shader::Shader(
  const char *v_path, 
  const char *f_path,
  const char *g_path)
{
  std::ifstream v_file, f_file, g_file;
  v_file.open(v_path); assert(v_file.is_open());
  f_file.open(f_path); assert(f_file.is_open());
  std::stringstream v_stream, f_stream, g_stream;

  v_stream << v_file.rdbuf();
  f_stream << f_file.rdbuf();

  v_file.close();
  f_file.close();
  const std::string vs = v_stream.str();
  const std::string fs = f_stream.str();

  const char *v_code = vs.c_str();
  const char *f_code = fs.c_str();

  int success;
  char info_log[512];

  // Vertex Shader
  unsigned int v_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(v_shader, 1, &v_code, NULL);
  glCompileShader(v_shader);
  glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(v_shader, 512, NULL, info_log);
    std::cerr << "Couldn't compile the vertex shader: "
             << info_log
             << std::endl;
  }

  // Fragment Shader
  unsigned int f_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f_shader, 1, &f_code, NULL);
  glCompileShader(f_shader);
  glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(f_shader, 512, NULL, info_log);
    std::cerr << "Couldn't compile the fragment shader: "
             << info_log
             << std::endl;
  }

  // Geometry shader
  unsigned int g_shader;
  if(g_path != nullptr) {
    g_file.open(g_path);
    assert(g_file.is_open());
    g_stream << g_file.rdbuf();
    g_file.close();
    const std::string gs = g_stream.str();
    const char* g_code = gs.c_str();

    g_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(g_shader, 1, &g_code, NULL);
    glCompileShader(g_shader);
    glGetShaderiv(g_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(g_shader, 512, NULL, info_log);
      std::cerr << "Couldn't compile the geometry shader: "
               << info_log
               << std::endl;
    }
  }

  // Shader program
  id_ = glCreateProgram();
  glAttachShader(id_, v_shader);
  if(g_path != nullptr) {
    glAttachShader(id_, g_shader);
  }
  glAttachShader(id_, f_shader);
  glLinkProgram(id_);

  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id_, 512, NULL, info_log);
    std::cerr << "Couldn't link the shaders: "
             << info_log
             << std::endl;  
  }
  glDeleteShader(v_shader);
  glDeleteShader(f_shader);
  if (g_path != nullptr)
    glDeleteShader(g_shader);
}

Shader* Shader::use() {
  glUseProgram(id_);
  return this;
}

void Shader::seti(
  const char *name, 
  const int val) const
{
  glUniform1i(glGetUniformLocation(id_, name), val);
}

void Shader::setf(
  const char *name, 
  const float val) const
{
  glUniform1f(glGetUniformLocation(id_, name), val);
}

void Shader::setmat4(
  const char *name, 
  const glm::mat4 val) const
{
  glUniformMatrix4fv(glGetUniformLocation(
    id_, name), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setvec2(
  const char *name, 
  const glm::vec2 val) const
{
  glUniform2fv(glGetUniformLocation(
    id_, name), 1, glm::value_ptr(val));
}

void Shader::setvec3(
  const char *name, 
  const glm::vec3 val) const
{
  glUniform3fv(glGetUniformLocation(
    id_, name), 1, glm::value_ptr(val));
}

void Shader::setuniform(
  const char *name, 
  const int binding_point) const
{
  glUniformBlockBinding(
    id_, glGetUniformBlockIndex(id_, name), binding_point);
}

} // namespace controls
