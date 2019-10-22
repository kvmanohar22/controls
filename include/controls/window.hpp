#ifndef _CONTROLS_WINDOW_H_
#define _CONTROLS_WINDOW_H_

#include "controls/global.hpp"
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
    init();
  }

  bool init();
  bool show();
  bool render();

private:
  size_t      width_;
  size_t      height_;
  std::string name_;
  Shader*     shader_;
  Renderer*   renderer_;
  Shader*     line_shader_;
  Renderer*   line_renderer_;
  CLTIS*      controller_;


  GLFWwindow* window_;
  GLFWwindow* shared_window_;
  std::thread render_thread_;
}; // class Window

} // namespace controls

#endif
