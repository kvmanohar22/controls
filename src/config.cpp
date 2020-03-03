#include "controls/config.hpp"

namespace controls {

Config::Config() :
    window_w_(1200),
    window_h_(800),
    n_particles_(10000),
    life_gradient_(0.99998),
    dt_(0.5e-4)
  {}

Config& Config::instance() {
  static Config instance_;
  return instance_;
}



} // namespace controls
