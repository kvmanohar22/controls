#ifndef _CONTROLS_WINDOW_H_
#define _CONTROLS_WINDOW_H_

#include "controls/global.hpp"
#include "controls/camera.hpp"
#include "controls/renderer.hpp"
#include "controls/shader.hpp"
#include "controls/linear_continuous_system.hpp"

#include <iostream>
#include <thread>
#include <mutex>

namespace controls {

class Window {
public:
  Window() =default;
 ~Window();
  explicit Window(size_t width, size_t height, std::string name, CLTIS* controller)
    : width_(width),
      height_(height),
      name_(name),
      controller_(controller)
  {
    glm::vec3 position = glm::vec3(28.87f, 15.44f, 20.08f);
    glm::vec3 target   = glm::vec3(-0.747516f, -0.436802f, -0.500423f);
    camera_ = new Camera(position, target);
    init();
  }

  bool init();
  void show();
  bool render();
  
  // Window management
  void process_input(GLFWwindow* window);

  size_t      width_;
  size_t      height_;
  std::string name_;
  
  // Point shader & renderer
  Shader*     shader_;
  Renderer*   renderer_;
 
  // Axis shader & renderer
  Shader*     axis_shader_;
  Renderer*   axis_renderer_;

  // Cube shader & renderer
  Shader*     cube_shader_;
  Renderer*   cube_renderer_;

  // Plane shader & renderer
  Renderer*   plane_renderer_;

  CLTIS*      controller_;
  Camera*     camera_;

  // Window management
  bool first_mouse_ ={true};
  float delta_time_ ={0.0f};
  float last_frame_ ={0.0f};
  double x_last_ ={0.0f};
  double y_last_ ={0.0f};

  GLFWwindow* window_;
  GLFWwindow* shared_window_;
  std::thread render_thread_;
}; // class Window


class WindowCallbackHelper {
public:
  static Window* window_instance;

  static void ScrollCallback(GLFWwindow *window, double dx, double dy)
  {
    window_instance->camera_->process_scroll(dy);
  }

  static void MouseCallback(GLFWwindow *window, double x_new, double y_new)
  {
    if (window_instance->first_mouse_) {
      window_instance->x_last_ = x_new;
      window_instance->y_last_ = y_new;
      window_instance->first_mouse_ = false;
    }
    float dx = x_new - window_instance->x_last_;
    float dy = window_instance->y_last_ - y_new;
    window_instance->x_last_ = x_new;
    window_instance->y_last_ = y_new;

    window_instance->camera_->process_mouse_movement(dx, dy);
  }

  static void setupScrollCallback(GLFWwindow* glfw_window, Window* window)
  {
    window_instance = window;
    glfwSetScrollCallback(glfw_window, WindowCallbackHelper::ScrollCallback);
  }

  static void setupMouseCallback(GLFWwindow* glfw_window, Window* window)
  {
    window_instance = window;
    glfwSetCursorPosCallback(glfw_window, WindowCallbackHelper::MouseCallback);
  }
};

} // namespace controls

#endif
