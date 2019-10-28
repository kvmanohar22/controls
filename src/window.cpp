#include "controls/window.hpp"
#include "glm/gtx/string_cast.hpp"

namespace controls {

Window* WindowCallbackHelper::window_instance = nullptr;

Window::~Window() {
  delete camera_;
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

void Window::process_input(GLFWwindow* window) {
  float current_frame = glfwGetTime();
  delta_time_ = current_frame - last_frame_;
  last_frame_ = current_frame;
  float camera_speed = 2.5f * delta_time_;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_->process_keyboard(camera_movement::FORWARD, delta_time_);
  else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_->process_keyboard(camera_movement::BACKWARD, delta_time_);
  else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_->process_keyboard(camera_movement::LEFT, delta_time_);
  else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_->process_keyboard(camera_movement::RIGHT, delta_time_);
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
  WindowCallbackHelper::setupScrollCallback(shared_window_, this);
  WindowCallbackHelper::setupMouseCallback(shared_window_, this);

  // projection and view matrices
  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), static_cast<float>(width_)/static_cast<float>(height_),
    0.1f, 100.0f);
  glm::mat4 view = camera_->get_view_matrix();

  shader_ = new Shader("../shaders/shader.vs",
      "../shaders/shader.fs", nullptr);
  axis_shader_ = new Shader("../shaders/axis_shader.vs",
      "../shaders/axis_shader.fs", "../shaders/axis_shader.gs");

  shader_->use();
  shader_->setmat4("projection", projection);
  shader_->setmat4("view", view);

  axis_shader_->use();
  axis_shader_->setmat4("projection", projection);
  axis_shader_->setmat4("view", view);

  renderer_ = new PointRenderer(shader_);
  axis_renderer_ = new AxisRenderer(axis_shader_);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE); // anti-aliasing
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glLineWidth(5.0f);

  // render the points!
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while(!glfwWindowShouldClose(shared_window_)) {
    process_input(shared_window_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = camera_->get_view_matrix();

    // Render the axes
    axis_shader_->use();
    axis_shader_->setmat4("view", view);
    dynamic_cast<AxisRenderer*>(axis_renderer_)->render();

    // Render the points
    shader_->use();
    shader_->setmat4("view", view);
    dynamic_cast<PointRenderer*>(renderer_)->render(
        controller_->x(), Eigen::Vector3d(1, 1, 1), 2.0f);

    glfwSwapBuffers(shared_window_);
    glfwPollEvents();
  }
  return true;
}

} // namespace controls
