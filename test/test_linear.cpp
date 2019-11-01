#include <iostream>
#include <thread>
#include <math.h>

#include "controls/global.hpp"
#include "controls/linear_continuous_system.hpp"
#include "controls/window.hpp"
#include "controls/utils.hpp"

using namespace controls;
using namespace std;
using namespace Eigen;

namespace {

void TestSingleParticle(Eigen::Matrix3d& A) {

  // initial position
  Particle* x = new Particle(12, 12, 12);
  vector<Particle*> xs;
  xs.push_back(x);

  controls::CLTIS* linear_controller = new controls::CLTIS(A, xs);
  linear_controller->summary();

  // for rendering
  controls::Window window(900, 600, "Control Theory", linear_controller); 
  window.show(); 

  while(true) {
    auto t = linear_controller->t();
    if(!linear_controller->step()) {
      cout << "Reached the final state!\n";
      break; 
    } 
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  delete linear_controller;
}

void TestSwarmOfParticles(Eigen::Matrix3d& A) {

  // initial position
  vector<Particle*> xs;
  xs.reserve(8);
  xs.push_back(new Particle(-12, -12, -12));
  xs.push_back(new Particle(-12, -12,  12));
  xs.push_back(new Particle(-12,  12, -12));
  xs.push_back(new Particle(-12,  12,  12));
  xs.push_back(new Particle( 12, -12, -12));
  xs.push_back(new Particle( 12, -12,  12));
  xs.push_back(new Particle( 12,  12, -12));
  xs.push_back(new Particle( 12,  12,  12));

  controls::CLTIS* linear_controller = new controls::CLTIS(A, xs);
  linear_controller->summary();

  // for rendering
  controls::Window window(900, 600, "Control Theory", linear_controller); 
  window.show(); 

  while(true) {
    auto t = linear_controller->t();
    if(!linear_controller->step()) {
      cout << "Reached the final state!\n";
      break; 
    } 
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  delete linear_controller;
}

void TestParticlesOnXYPlane(Eigen::Matrix3d& A) {

  // initial position
  vector<Particle*> xs;
  xs.reserve(72);
  double R = 24, theta=0, d_theta=3;
  double x, y;
  while (theta < 360) {
    x = R * cos(theta * controls::PI / 180.0);
    y = R * sin(theta * controls::PI / 180.0);
    xs.push_back(new Particle(x, y, 0));
    theta += d_theta;
  }

  cout << "Simulating " << xs.size() << " points\n";
  controls::CLTIS* linear_controller = new controls::CLTIS(A, xs);
  linear_controller->summary();

  // for rendering
  controls::Window window(900, 600, "Control Theory", linear_controller); 
  window.show(); 

  while(true) {
    auto t = linear_controller->t();
    if(!linear_controller->step()) {
      cout << "Reached the final state!\n";
      break; 
    } 
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  delete linear_controller;
}

}

void getA(Matrix3d& A, int type) {
  switch(type) {
    case 0: // stable
      A << -1, 0, 0, 0, -2, 0, 0, 0, -3;
      break;
    case 1: // unstable
      A << -1, 0, 0, 0, -2, 0, 0, 0, 3;
      break;
    case 2: // complex eigenvalues with real < 0
      A << -2, 0, 0, 0, -4, -1, 0, 5, 0;
      break;
    case 3: // complex eigenvalues with real = 0
      A << -2, 0, 0, 0, 0, -2, 0, 2, 0;
      break;
    case 4: // complex eigenvalues with real > 0
      A << -2, 0, 0, 0, 4, -1, 0, 5, 0;
      break;
 
  }
}


int main() {
  srand(static_cast<unsigned> (time(0)));
  Eigen::Matrix3d A(3,3);

  getA(A, 2);

  ::TestParticlesOnXYPlane(A);
  // ::TestSingleParticle(A);
  // ::TestSwarmOfParticles(A);
}

