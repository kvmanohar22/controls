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

// Samples points regularly on a 2D grid on XZ plane
inline vector<Vector3d> regularXZ(size_t n_points, float pmin, float pmax) {
  vector<Vector3d> pts;
  pts.reserve(n_points*n_points); 
  float dx = (pmax-pmin)/n_points;
  for(size_t i=0; i<n_points; ++i) {
    float xcurr = pmin+dx*i; 
    for(size_t j=0; j<n_points; ++j) {
      float zcurr = pmin+dx*j;
      Vector3d pt(xcurr, 0.0, zcurr);
      pts.push_back(pt);
    }
  }
  return pts;
}

} // namespace controls

#endif

