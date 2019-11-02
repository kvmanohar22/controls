#include "controls/config.hpp"

namespace controls {

Config::Config() :
    window_w_(1200),
    window_h_(800),
    n_particles_(300),
    life_gradient_(0.995f)
  {}

Config& Config::instance() {
  static Config instance_;
  return instance_;
}



} // namespace controls

