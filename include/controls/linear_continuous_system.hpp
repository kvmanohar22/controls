#ifndef _CONTROLS_LINEAR_H_
#define _CONTROLS_LINEAR_H_

#include "controls/global.hpp"
#include "controls/config.hpp"
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
     start_x_(x_),
     life_(1.0f)
  {
    col_ = color(); 
  }

  Particle(double x, double y, double z, Vector3d col)
   : x_(Vector3d(x, y, z)),
     col_(col),
     start_x_(x_),
     life_(1.0f)
  {
  }

  Particle(Vector3d parent_x)
   : x_(parent_x),
     start_x_(parent_x),
     life_(1.0f)
  {
    col_ = color(); 
  }

  Particle(const Particle& particle)
   : x_(particle.x_),
     start_x_(particle.x_),
     col_(particle.col_),
     life_(1.0f)
  {}

  Vector3d x_;
  Vector3d start_x_;
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
typedef vector<pair<Particle*, list<Particle*>>> PARTICLE_TRAIL;
typedef std::unique_lock<std::mutex> lock_t;

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
      list<Particle*> initial_particles;
      initial_particles.push_back(new Particle(*x[i])); 
      x_.push_back(make_pair(x[i], initial_particles)); 
    } 
  }

 ~CLTIS() {
    std::for_each(x_.begin(), x_.end(), [&](pair<Particle*, list<Particle*>>& x_pair) {
      delete x_pair.first;
      std::for_each(x_pair.second.begin(), x_pair.second.end(), [&](Particle* p) {
        delete p;
      });
    });
    cout << "So long, and thanks for all the fish!\n";
  }

  /* TODO: Interpolate if the points are far away */
  void update_particles(Particle* parent, list<Particle*>& particles) {
    const auto prevpos = particles.back()->x_;
    const auto currpos = parent->x_; 

    
    // decrease the life of the particle
    std::for_each(particles.begin(), particles.end(), [&](Particle* p) {
      p->life_ *= Config::life_gradient();
    });
  
    // We only maintain certain number of points in the trail
    if(particles.size() >= Config::n_particles()) {
      particles.pop_front();
      particles.push_back(new Particle(*parent)); 
    } else {
      particles.push_back(new Particle(*parent)); 
    }
  }

  bool step() {
    // Has reached the origin, stop updating
    {
      lock_t lock(x_mutex_); 

      size_t k=0;
      for(size_t i=0; i<x_.size(); ++i, ++k) {
        if (x_[i].first->x_.norm() > 1e-3) {
          break;
        }
      }
      if (k == x_.size()) {
        stop_exec_ = true;
        return false;
      }

      t_ += Config::dt();
      Matrix3d A_exp = (A_*t_).exp();
      vel_ = A_*x_.at(0).first->x_;
      for(size_t i=0; i<x_.size(); ++i) {
        x_.at(i).first->x_ = A_exp * x_.at(i).first->start_x_;
        update_particles(x_.at(i).first, x_.at(i).second);
      }
      return true;
    }
  }

  void summary() {
    cout << "State Transition Matrix = \n" << A_ << endl;
    cout << "Eigen values = " << A_.eigenvalues().transpose() << endl;
  }

  inline Matrix3d       A() { return A_; }
  inline double         t() { return t_; }
  inline PARTICLE_TRAIL x()
  {
    {
      lock_t lock(x_mutex_); 
      return x_; 
    } 
  }
  inline bool stop()
  {
    {
      lock_t lock(stop_mutex_); 
      return stop_exec_; 
    } 
  }

private:
  Matrix3d A_;
  PARTICLE_TRAIL x_;
  Vector3d vel_;
  double t_={0};
  bool stop_exec_={false};
  std::mutex x_mutex_;
  std::mutex stop_mutex_;
};

} // namespace controls

#endif

