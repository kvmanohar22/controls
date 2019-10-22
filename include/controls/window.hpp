#ifndef _CONTROLS_WINDOW_H_
#define _CONTROLS_WINDOW_H_

#include "controls/global.hpp"
#include "controls/renderer.hpp"
#include "controls/shader.hpp"

#include <iostream>
#include <thread>
#include <mutex>

namespace controls {

class Window {
public:
  Window() =default;
 ~Window();
  explicit Window(size_t width, size_t height, std::string name)
    : width_(width),
      height_(height),
      name_(name)
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
  Renderer*   renderer_;
  Shader*     shader_;

  GLFWwindow* window_;
  GLFWwindow* shared_window_;
  std::thread render_thread_;
}; // class Window

} // namespace controls

#endif
