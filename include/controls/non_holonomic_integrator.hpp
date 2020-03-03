#ifndef _CONTROLS_NON_HOLO_H_
#define _CONTROLS_NON_HOLO_H_

#include "controls/linear_continuous_system.hpp"
#include "controls/global.hpp"
#include "controls/config.hpp"
#include "controls/utils.hpp"

#include <list>
#include <mutex>
#include <cmath>

namespace controls {

class NonHolonomicIntegratorSingle {
public: 
  NonHolonomicIntegratorSingle(size_t id, double a, double c)
  : index(id), a_(a), c_(c)
  {
    x_.resize(1);
    x_[0].first = new controls::Particle(0, 0, 0);
    x_[0].second = list<Particle*>();
    
    // transition matrix
    H_(0, 0) = 0;
    H_(0, 1) = -c;
    H_(1, 0) =  c;
    H_(1, 1) = 0;
  
    I_ = Matrix2d::Identity();
    u0 = a_ * c_;

    // initial input
    u0_(0) = std::sqrt(u0/2);
    u0_(1) = std::sqrt(u0/2); 
    // u0_(0) = 0;
    // u0_(1) = std::sqrt(u0); 
  }

 ~NonHolonomicIntegratorSingle() {
    std::for_each(x_.begin(), x_.end(), [&](pair<Particle*, list<Particle*>>& x_pair) {
      delete x_pair.first;
      std::for_each(x_pair.second.begin(), x_pair.second.end(), [&](Particle* p) {
        delete p;
      });
    });
    cout << "So long, and thanks for all the fish!\n";
  }

  void update_particles(Particle* parent, list<Particle*>& particles) {
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

  bool print() {
    if ((std::abs(t_ - 0.25) < 1e-9) || (std::abs(t_ - 0.5) < 1e-9) || (std::abs(t_ - 0.75) < 1e-9) || (std::abs(t_ - 1) < 1e-9))
      return true;
    return false;
  }

  bool step() {
    {
      lock_t lock(x_mutex_); 

      t_ += Config::dt();
      if (t_ < 1) {
        for(size_t i=0; i<x_.size(); ++i) {
          x_.at(i).first->x_.head(2) = ((H_*t_).exp() - I_) * H_.inverse() * u0_;
          x_.at(i).first->x_(2) = (u0_.transpose() * (I_ * t_ - H_.inverse() * (H_ * t_).exp() + H_.inverse()) * u0_ );
          x_.at(i).first->x_(2) /= c_;
          update_particles(x_.at(i).first, x_.at(i).second);
          if (print()) {
            std::cout << "ID = " << index << "\t t = " << t_ << "\t X = [" << x_.at(i).first->x_.transpose() << "]" << std::endl;
          }
        }
        return true;
      }
    }
  }

  void summary() {}

  inline controls::PARTICLE_TRAIL x()
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
  PARTICLE_TRAIL x_;
  Eigen::Matrix2d I_;
  Eigen::Matrix2d H_;
  Eigen::Vector2d u0_; 
  double u0;
  size_t index;
  double a_;
  double c_;
  double t_={0};
  bool stop_exec_={false};
  std::mutex x_mutex_;
  std::mutex stop_mutex_;
};

class NonHolonomicIntegrator {
public: 
  NonHolonomicIntegrator(vector<double> a, vector<double> c) {
    particles_.resize(a.size());
    for (size_t i=0; i<a.size(); ++i) {
      particles_[i] = new NonHolonomicIntegratorSingle(i, a[i], c[i]);
    }
  }

 ~NonHolonomicIntegrator() {
    for (size_t i=0; i<particles_.size(); ++i) {
      delete particles_[i];
    }
  }

  bool step() {
    for (size_t i=0; i<particles_.size(); ++i) {
      particles_[i]->step();
    }
    if (particles_[0]->print())
      std::cout << "--------------------------\n";
    return true;
  }

  inline bool stop() {
    for (size_t i=0; i<particles_.size(); ++i) {
      if (particles_[i]->stop())
        return true;
    }
    return false;
  }

  inline controls::PARTICLE_TRAIL x()
  {
    PARTICLE_TRAIL trails(particles_.size());
    for (size_t i=0; i<particles_.size(); ++i) {
      PARTICLE_TRAIL new_trail;
      new_trail = particles_[i]->x();
      trails[i] = new_trail[0];
    }
    return trails;
  }

private:
  vector<NonHolonomicIntegratorSingle*> particles_;
};

} // namespace controls

#endif
