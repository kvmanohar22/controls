#ifndef _CONTROLS_LINEAR_H_
#define _CONTROLS_LINEAR_H_

#include "controls/global.hpp"
#include "controls/utils.hpp"
#include <mutex>

namespace controls {

/*
 * A particle that represents trail of x(t)
 */

class Particle {
public:
  Particle()
   : x_(Vector3d(0, 0, 0)),
     col_(Vector3d(1, 0, 0)),
     life_(1.0f)
  {}
 
  Particle(double x, double y, double z)
   : x_(Vector3d(x, y, z)),
     life_(1.0f)
  {
    col_ = color(); 
    cout << "color = " << col_.transpose() << endl;
  }

  Particle(Vector3d parent_x)
   : x_(parent_x),
     life_(1.0f)
  {
    col_ = color(); 
  }

  Particle(const Particle& particle)
   : x_(particle.x_),
     col_(particle.col_),
     life_(1.0f)
  {
    /* 
    // randomly initialize the position
    double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    Vector3d random_offset_v(r,r,r);
    x_ += random_offset_v;
    */
  }

  Vector3d x_;
  Vector3d col_;
  float life_;
};

/*
 * Continuous Linear Time Invariant System
 * x' = Ax
 *
 * Solution to the above system;
 * x(t) = e^(At)x(0)
 *
 */

// TODO: Why vector of pair? maybe list of pair?
typedef vector<pair<Particle*, vector<Particle*>>> PARTICLE_TRAIL;
class CLTIS {
public: 
  CLTIS() {
    A_ = Matrix3d::Identity();
  }

  CLTIS(Matrix3d A)
    : A_(A)
  {}

  CLTIS(Matrix3d A, vector<Particle*> x)
    : A_(A)
  {
    x_.reserve(x.size());
    for(size_t i=0; i<x.size(); ++i) {
      vector<Particle*> initial_particles; 
      initial_particles.reserve(0x100);
      initial_particles.push_back(new Particle(*x[i])); 
      x_.push_back(make_pair(x[i], initial_particles)); 
    } 
  }

 ~CLTIS() {
    std::for_each(x_.begin(), x_.end(), [&](pair<Particle*, vector<Particle*>>& x_pair) {
      delete   x_pair.first;
      for(size_t i=0; i<x_pair.second.size(); ++i)
        delete x_pair.second[i]; 
    });
  }

  void update_particles(Particle* parent, vector<Particle*>& particles) {
    const auto prevpos = particles.back()->x_;
    const auto currpos = parent->x_; 
    
    particles.push_back(new Particle(*parent)); 
  }

  bool step() {
    // Has reached the origin, stop updating 
    if (x_[0].first->x_.norm() < 1e-3)
      return false; 

    t_ += controls::dt;
    Matrix3d A_exp = (A_*t_).exp();
    vel_ = A_*x_.at(0).first->x_;
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_ = A_exp * x_.at(i).first->x_;
      update_particles(x_.at(i).first, x_.at(i).second);
    }
    return true;
  }

  void summary() {
    cout << "State Transition Matrix = \n" << A_ << endl;
    cout << "Eigen values = " << A_.eigenvalues().transpose() << endl;
  }

  inline Matrix3d       A() { return A_; }
  inline double         t() { return t_; }
  inline PARTICLE_TRAIL x() { 
    {
      unique_lock<mutex> lock(x_lock_); 
      return x_; 
    } 
  }

private:
  Matrix3d A_;
  PARTICLE_TRAIL x_;
  Vector3d vel_;
  double t_={0};
  mutex x_lock_;
};

} // namespace controls

#endif

