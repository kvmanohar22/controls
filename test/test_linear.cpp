#include <iostream>
#include <thread>
#include <math.h>

#include "controls/global.hpp"
#include "controls/linear_continuous_system.hpp"
#include "controls/window.hpp"
#include "controls/utils.hpp"
#include "controls/config.hpp"

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
  controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", linear_controller); 
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

void interpolatePoints(vector<Particle*>& particles,
    size_t src_idx, size_t dst_idx, const size_t dim) {
  const size_t n_intermediates=5;
  const Particle* src = particles[src_idx]; 
  const Particle* dst = particles[dst_idx]; 
  const float h = std::fabs(dst->x_(dim)-src->x_(dim))/n_intermediates; 
  for(size_t i=1; i<n_intermediates; ++i) {
    Vector3d newxyz(src->x_.x(), src->x_.y(), src->x_.z());
    newxyz(dim) += (i*h);
    particles.push_back(new Particle(newxyz));
  }
}

void TestSwarmOfParticles(Eigen::Matrix3d& A) {

  // initial position
  vector<Particle*> xs;
  xs.reserve(8);
  xs.push_back(new Particle(-12, -12, -12)); // 0
  xs.push_back(new Particle(-12, -12,  12));
  xs.push_back(new Particle(-12,  12, -12)); // 2
  xs.push_back(new Particle(-12,  12,  12));
  xs.push_back(new Particle( 12, -12, -12)); // 4
  xs.push_back(new Particle( 12, -12,  12));
  xs.push_back(new Particle( 12,  12, -12)); // 6
  xs.push_back(new Particle( 12,  12,  12));

  interpolatePoints(xs, 1, 5, 0);
  interpolatePoints(xs, 0, 4, 0);
  interpolatePoints(xs, 3, 7, 0);
  interpolatePoints(xs, 2, 6, 0);

  interpolatePoints(xs, 4, 6, 1); 
  interpolatePoints(xs, 5, 7, 1); 
  interpolatePoints(xs, 1, 3, 1); 
  interpolatePoints(xs, 0, 2, 1); 

  interpolatePoints(xs, 4, 5, 2);
  interpolatePoints(xs, 0, 1, 2);
  interpolatePoints(xs, 6, 7, 2);
  interpolatePoints(xs, 2, 3, 2);
  
  controls::CLTIS* linear_controller = new controls::CLTIS(A, xs);
  linear_controller->summary();

  // for rendering
  controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", linear_controller); 
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
  double R = 24, theta=0, d_theta=1;
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
  controls::Window window(Config::window_w(), Config::window_h(), "Control Theory", linear_controller); 
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

  getA(A, 3);

  // ::TestParticlesOnXYPlane(A);
  // ::TestSingleParticle(A);
  ::TestSwarmOfParticles(A);
}

