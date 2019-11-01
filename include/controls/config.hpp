#ifndef _CONTROLS_CONFIG_H_
#define _CONTROLS_CONFIG_H_

#include "controls/global.hpp"

namespace controls {

class Config {
public:
  static Config& instance();
  static size_t& window_w() { return instance().window_w_; }
  static size_t& window_h() { return instance().window_h_; }
  static size_t& n_particles() { return instance().n_particles_; }
  static float&  life_gradient() { return instance().life_gradient_; }

private:
  Config();
  Config(Config& c);
  void operator = (Config& c); 
  
  size_t window_w_;    // Render window width
  size_t window_h_;    // Render window height
  size_t n_particles_; // number of trailing particles
  float life_gradient_; // decrease particle alpha by this factor
};

} // namespace controls

#endif

