#include <iostream>
#include <thread>
#include <math.h>

#include "controls/global.hpp"
#include "controls/non_holonomic_integrator.hpp"
#include "controls/window.hpp"
#include "controls/utils.hpp"
#include "controls/config.hpp"

using namespace controls;
using namespace std;
using namespace Eigen;

namespace {

  void test0() {
    NonHolonomicIntegrator* controller = new NonHolonomicIntegrator();

    // for rendering
    controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", controller); 
    window.show(); 

    while (true) {
      controller->step();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}

int main() {
  srand(static_cast<unsigned> (time(0)));
  test0();
} 


