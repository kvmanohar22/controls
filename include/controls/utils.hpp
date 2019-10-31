#ifndef CONTROLS_UTILS_H_
#define CONTROLS_UTILS_H_

#include "controls/global.hpp"

namespace controls {

template <typename T>
vector<T> linspace(T a, T b, size_t N) {
  T h = (b-a)/static_cast<T>(N-1);
  vector<T> xs(N);
  T v = a;
  for(auto& itr: xs) {
    itr = v;
    v += h;
  }
  return xs;
}

inline Vector3d color() {
  Vector3d rgb;
  for(size_t i=0; i<3; ++i) 
    rgb(i) = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
  
  return rgb;
}


} // namespace controls

#endif

