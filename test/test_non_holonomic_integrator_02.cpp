#include <iostream>
#include <cstdlib>
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
    vector<double> a(1), c(1);

    a[0] = 1;
    c[0] = 2*PI;
    NonHolonomicIntegrator* controller = new NonHolonomicIntegrator(a, c);

    // for rendering
    controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", controller); 
    window.show(); 

    while (true) {
      controller->step();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  void test1() {
    size_t num = 15;
    vector<double> a(num, 1.0), c(num);
    for (size_t i=0; i<num; ++i) {
      c[i] = (i+1) * 2 * PI;
    }
    NonHolonomicIntegrator* controller = new NonHolonomicIntegrator(a, c);

    // for rendering
    controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", controller); 
    window.show(); 

    while (true) {
      controller->step();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  // (0, 0, 0) -> (3, 3, 5) [single particle]
  void test2() {
    size_t num = 15;
    vector<double> a(num, 1.0), c(num);
    for (size_t i=0; i<num; ++i) {
      c[i] = (i+1) * 2 * PI;
    }
    NonHolonomicIntegrator* controller = new NonHolonomicIntegrator(a, c);

    // for rendering
    controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", controller); 
    window.show(); 

    while (true) {
      controller->step();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  // (0, 0, 0) -> (3, 3, 5) [multiple particles]
  void test3() {
    size_t num = 15;
    vector<double> a(num, 1.0), c(num);
    for (size_t i=0; i<num; ++i) {
      c[i] = (i+1) * 2 * PI;
    }
    NonHolonomicIntegrator* controller = new NonHolonomicIntegrator(a, c);

    // for rendering
    controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", controller); 
    window.show(); 

    while (true) {
      controller->step();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      if (controller->stop()) {
        break;
      }
    }
  }
}

int main(int argc, char** argv) {
  srand(static_cast<unsigned> (time(nullptr)));

  if (argc == 1) {
    std::cerr << "[ERROR] Usage: ./test_non_holonomic_integrator <test_number>" << std::endl;
    return -1;
  }

  int test_number = atoi(argv[1]);

  switch (test_number) {
    case 0:
      test0();
      break;
    case 1:
      test1();
      break;
    case 2:
      test2();
      break;
    case 3:
      test3();
      break;
  }
} 


