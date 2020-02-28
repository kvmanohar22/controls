#include "controls/config.hpp"

namespace controls {

Config::Config() :
    window_w_(1200),
    window_h_(800),
    n_particles_(1000),
    life_gradient_(0.995f),
    dt_(1e-3)
  {}

Config& Config::instance() {
  static Config instance_;
  return instance_;
}



} // namespace controls

