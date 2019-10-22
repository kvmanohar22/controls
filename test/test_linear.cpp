#include <iostream>
#include <thread>

#include "controls/linear_continuous_system.hpp"

using namespace controls;
using namespace std;

int main() {
  controls::CLTIS linear_controller;  
  linear_controller.summary();

  while(true) {
    auto t = linear_controller.t();
    auto x = linear_controller.step();
    cout << "t = " << t << "\t x = " << x.transpose() << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

