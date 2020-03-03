#include "controls/window.hpp"
#include "glm/gtx/string_cast.hpp"

namespace controls {

Window* WindowCallbackHelper::window_instance = nullptr;

Window::~Window() {
  cout << "Render thread stop...\n";
  render_thread_.join();
  delete camera_;
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

void Window::show() {
  render_thread_ = std::thread(&Window::render, this);
  cout << "Render thread start...\n";
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
  glfwSetInputMode(shared_window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
  glfwSetErrorCallback([](int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
  });
  WindowCallbackHelper::setupScrollCallback(shared_window_, this);
  WindowCallbackHelper::setupMouseCallback(shared_window_, this);

  // projection and view matrices   
  glm::mat4 projection = glm::perspective(
    glm::radians(45.0f), static_cast<float>(width_)/static_cast<float>(height_),
    0.1f, 1000.0f);
   
  glm::mat4 view = camera_->get_view_matrix();

  shader_ = new Shader("../shaders/point_instances/point.vert",
      "../shaders/point_instances/point.frag", nullptr);
  axis_shader_ = new Shader("../shaders/axis_shader.vert",
      "../shaders/axis_shader.frag", "../shaders/axis_shader.geom");
  cube_shader_ = new Shader("../shaders/line/line.vert",
      "../shaders/line/line.frag", "../shaders/line/line.geom");

  // point shader & renderer
  shader_->use();
  shader_->setmat4("projection", projection);
  shader_->setmat4("view", view);
  renderer_ = new PointRenderer(shader_);

  // axis shader & renderer
  axis_shader_->use();
  axis_shader_->setmat4("projection", projection);
  axis_shader_->setmat4("view", view);
  axis_renderer_ = new AxisRenderer(axis_shader_);

  // cube shader & renderer
  cube_shader_->use();
  cube_shader_->setmat4("projection", projection);
  cube_shader_->setmat4("view", view);
  cube_renderer_ = new CubeRenderer(cube_shader_);

  // Plane
  plane_renderer_ = new PlaneRenderer(cube_shader_);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);        // anti-aliasing
  glEnable(GL_LINE_SMOOTH);        // smooth-out solid lines
  glEnable(GL_PROGRAM_POINT_SIZE); // enable to change point-size
  glLineWidth(1.0f);

  // render the points!
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while(!glfwWindowShouldClose(shared_window_) && !controller_->stop()) {
    process_input(shared_window_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = camera_->get_view_matrix(cam_type::STATIC);

    // Render the axes
    axis_shader_->use();
    axis_shader_->setmat4("view", view);
    dynamic_cast<AxisRenderer*>(axis_renderer_)->render();

    // Render the points along with the trail
    shader_->use();
    shader_->setmat4("view", view);
    PARTICLE_TRAIL particles = controller_->x();
    dynamic_cast<PointRenderer*>(renderer_)->render(
       particles);

    // Render the cube
    cube_shader_->use();
    cube_shader_->setmat4("view", view);
    // dynamic_cast<CubeRenderer*>(cube_renderer_)->render();

    // Render the plane
    cube_shader_->use();
    cube_shader_->setmat4("view", view);
    dynamic_cast<PlaneRenderer*>(plane_renderer_)->render();

    glfwSwapBuffers(shared_window_);
    glfwPollEvents();
  }
  return true;
}

} // namespace controls
