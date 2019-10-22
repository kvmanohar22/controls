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

  // projection and view matrices
  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), static_cast<float>(width_)/static_cast<float>(height_),
    0.1f, 100.0f);
  /* TODO: Change the view matrix dynamically */
  glm::mat4 view = glm::lookAt(
    glm::vec3(0, 0, 100),
    glm::vec3(0.0f, 0.0f, 0.0f), 
    glm::vec3(0.0f, 1.0f, 0.0f));

  shader_ = new Shader("../shaders/shader.vs", "../shaders/shader.fs", nullptr);
  line_shader_ = new Shader("../shaders/shader.vs", "../shaders/shader.fs", "../shaders/line.gs");

  shader_->use();
  shader_->setmat4("projection", projection);
  shader_->setmat4("view", view);

  line_shader_->use();
  line_shader_->setmat4("projection", projection);
  line_shader_->setmat4("view", view);

  renderer_ = new Renderer(shader_);
  line_renderer_ = new Renderer(line_shader_);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_PROGRAM_POINT_SIZE);

  // render the points!
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while(!glfwWindowShouldClose(shared_window_)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the axes
    line_renderer_->render(Vector3d(0, 0, 0), 1.0f, Vector3d(0, 1, 0));

    // Render the points    
    renderer_->render(controller_->x() , 5.0f, Eigen::Vector3d(1, 1, 1));

    glfwSwapBuffers(shared_window_);
    glfwPollEvents();
  }
  return true;
}

} // namespace controls
