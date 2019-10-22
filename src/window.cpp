#include "controls/window.hpp"
#include "glm/gtx/string_cast.hpp"

namespace controls {

Window::~Window() {
  render_thread_.join();
}

bool Window::init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  window_ = glfwCreateWindow(1, 1, "", NULL, NULL);
  if (window_ == NULL) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window_);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(1);
  gl3wInit();
  return true;
}

bool Window::show() {
  render_thread_ = std::thread(&Window::render, this);
  return true;
}

bool Window::render() {
  glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
  shared_window_ = glfwCreateWindow(width_, height_, name_.c_str(), NULL, window_);
  if (shared_window_ == NULL) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(shared_window_);
  glfwSetWindowUserPointer(shared_window_, this);
  glfwSetErrorCallback([](int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
  });

  /* TODO: Define projection matrix */
  glm::mat4 projection = glm::ortho(
    0.0f, static_cast<float>(width_),
    static_cast<float>(height_), 0.0f,
    -1.0f, 1.0f);

  shader_ = new Shader("../shaders/shader.vs", "../shaders/shader.fs");
  renderer_ = new Renderer(shader_);

  shader_->use();
  shader_->seti("image", 0);
  shader_->setmat4("projection", projection);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_PROGRAM_POINT_SIZE);

  // render mosaic!
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while(!glfwWindowShouldClose(shared_window_)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* TODO: Render the evolution of points */

    glfwSwapBuffers(shared_window_);
    glfwPollEvents();
  }
  return true;
}

} // namespace controls
