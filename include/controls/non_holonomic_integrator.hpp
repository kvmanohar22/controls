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

class NonHolonomicIntegrator {
public: 
  NonHolonomicIntegrator() {
    x_.resize(1);
    x_[0].first = new controls::Particle(0, 0, 0);
    x_[0].second = list<Particle*>();
    
    // transition matrix
    H_(0, 0) = 0;
    H_(0, 1) = -c;
    H_(1, 0) =  c;
    H_(1, 1) = 0;
  
    I_ = Matrix2d::Identity();

    // initial input 
    u0_(0) = std::sqrt(u0); 
    u0_(1) = std::sqrt(u0); 
  }

 ~NonHolonomicIntegrator() {
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

  bool step() {
    {
      lock_t lock(x_mutex_); 

      t_ += Config::dt();
      for(size_t i=0; i<x_.size(); ++i) {
        x_.at(i).first->x_.head(2) = ((H_*t_).exp() - I_) * H_.inverse() * u0_;
        x_.at(i).first->x_(2) = (u0_.transpose() * (I_ * t_ - H_.inverse() * (H_ * t_).exp() + H_.inverse()) * u0_ );
        x_.at(i).first->x_(2) /= c;
        update_particles(x_.at(i).first, x_.at(i).second);
        std::cout << "t = " << t_ << "\t" << x_.at(i).first->x_.transpose() << std::endl;
      }
      return true;
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
  double u0 = {2*PI*1};
  double c  = {2*PI};
  double t_={0};
  bool stop_exec_={false};
  std::mutex x_mutex_;
  std::mutex stop_mutex_;
};

} // namespace controls

#endif

