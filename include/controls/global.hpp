#ifndef _CONTROLS_GLOBAL_H_
#define _CONTROLS_GLOBAL_H_

#include "gl3w/gl3w.h"
#include <GLFW/glfw3.h>

#include <Eigen/StdVector>
#include <Eigen/src/Core/Matrix.h>
#include <unsupported/Eigen/MatrixFunctions>

#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

namespace controls {
  using namespace std;
  using namespace Eigen;

  static constexpr double PI = 3.1415926535;
  static constexpr double dt = 1e-7;
}

#endif
