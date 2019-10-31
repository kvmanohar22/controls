#ifndef _CONTROLS_LINEAR_H_
#define _CONTROLS_LINEAR_H_

#include "controls/global.hpp"

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
     col_(Vector3d(1, 0, 0)),
     life_(1.0f)
  {}

  Particle(Vector3d parent_x)
   : x_(parent_x),
     col_(Vector3d(1, 0, 0)),
     life_(1.0f)
  {}


  Particle(const Particle& particle)
   : x_(particle.x_),
     col_(Vector3d(1.0f, 0.0f, 0.0f)),
     life_(1.0f)
  {
    // randomly initialize the position
    double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    Vector3d random_offset_v(r,r,r);
    x_ += random_offset_v;
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
      vector<Particle*> initial_particles(10, new Particle(*x[i])); 
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
    vector<Particle*>::iterator itr=particles.begin();
    for(; itr!= particles.end(); ++itr) {
      (*itr)->life_ -= 0.05;
      (*itr)->x_ += vel_*dt*3.0;
    } 
  }

  PARTICLE_TRAIL step() {
    t_ += controls::dt;
    Matrix3d A_exp = (A_*t_).exp();
    vel_ = A_*x_.at(0).first->x_;
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_ = A_exp * x_.at(i).first->x_;
      update_particles(x_.at(i).first, x_.at(i).second);
    }
    return x_;
  }

  void summary() {
    cout << "State Transition Matrix = \n" << A_ << endl;
    cout << "Eigen values = " << A_.eigenvalues().transpose() << endl;
  }

  inline Matrix3d       A() { return A_; }
  inline PARTICLE_TRAIL x() { return x_; }
  inline double         t() { return t_; }

private:
  Matrix3d A_;
  PARTICLE_TRAIL x_;
  Vector3d vel_;
  double t_={0};
};

} // namespace controls

#endif

