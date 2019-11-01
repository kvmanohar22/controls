#include "controls/config.hpp"

namespace controls {

Config::Config() :
    window_w_(900),
    window_h_(600),
    n_particles_(200),
    life_gradient_(0.99f)
  {}

Config& Config::instance() {
  static Config instance_;
  return instance_;
}



} // namespace controls

