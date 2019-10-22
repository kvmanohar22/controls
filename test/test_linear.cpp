#include <iostream>
#include <thread>

#include "controls/linear_continuous_system.hpp"
#include "controls/window.hpp"

using namespace controls;
using namespace std;

int main() {
  // State transfer function
  // Note: All eigenvalues are strictly negative => stable system
  //       So, x(t) -> 0 as t -> inf
  Eigen::Matrix3d A(3,3);
  A << -1, 0, 0, 0, -2, 0, 0, 0, -3;

  // initial position
  Eigen::Vector3d x(20, 20, 20);

  controls::CLTIS *linear_controller = new controls::CLTIS(A, x);
  linear_controller->summary();

  // for rendering
  controls::Window window(900, 600, "Control Theory", linear_controller); 
  window.show(); 

  while(true) {
    auto t = linear_controller->t();
    auto x = linear_controller->step();
    cout << "t = " << t << "\t x = " << x.transpose() << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  delete linear_controller;
}

