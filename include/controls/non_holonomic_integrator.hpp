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

enum class InputType {
  Optimal,
  Legendre13, // x3 will go to zero after every cycle
  Legendre23,
  Legendre24, // x3 will go to zero after every cycle
  Legendre34,
  Legendre46, // x3 will go to zero after every cycle
};

class NonHolonomicIntegratorSingle {
public: 
  NonHolonomicIntegratorSingle(size_t id, double a, double c, Vector3d col, InputType type=InputType::Optimal)
  : index(id), a_(a), c_(c), type_(type)
  {
    x_.resize(1);
    x_[0].first = new controls::Particle(0, 0, 0, col);
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

  NonHolonomicIntegratorSingle(size_t id, double a, double c, InputType type=InputType::Optimal)
  : index(id), a_(a), c_(c), type_(type)
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
    return true;
    if ((std::abs(t_ - 0.25) < 1e-9) || (std::abs(t_ - 0.5) < 1e-9) || (std::abs(t_ - 0.75) < 1e-9) || (std::abs(t_ - 1) < 1e-9))
      return true;
    return false;
  }

  // sinosuidal input [this is optimal when frequency is 2PI]
  void sinosuidal() {
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_.head(2) = ((H_*t_).exp() - I_) * H_.inverse() * u0_;
      x_.at(i).first->x_(2) = (u0_.transpose() * (I_ * t_ - H_.inverse() * (H_ * t_).exp() + H_.inverse()) * u0_ );
      x_.at(i).first->x_(2) /= c_;
      update_particles(x_.at(i).first, x_.at(i).second);
      if (print()) {
        std::cout << "ID = " << index << "\t t = " << t_ << "\t X = [" << x_.at(i).first->x_.transpose() << "]" << std::endl;
      }
    }
  }

  void Legendre13() {
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_(0) = (pow(t_, 2) - 1) / 2.0;
      x_.at(i).first->x_(1) = (1.25 * pow(t_, 4) - 1.5 * pow(t_, 2) + 0.25) / 2.0;
      x_.at(i).first->x_(2) = ((5.0/6.0)*pow(t_, 6)-2.5*pow(t_, 4)+pow(t_, 2)-((5.0/6.0)-2.5+1))/8.0;
      update_particles(x_.at(i).first, x_.at(i).second);
      if (print()) {
        std::cout << "ID = " << index << "\t t = " << t_ << "\t X = [" << x_.at(i).first->x_.transpose() << "]" << std::endl;
      }
    }
  }

  void Legendre23() {
    static double K2 = 35.0/4.0;
    static double K1 = 1.0;
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_(0) = K1 * ((pow(t_, 3) - t_) / 2.0);
      x_.at(i).first->x_(1) = K2 * ((1.25 * pow(t_, 4) - 1.5 * pow(t_, 2) + 0.25) / 2.0);
      x_.at(i).first->x_(2) = K1 * K2 * (((5.0/7.0) * pow(t_, 7) - (9.0/5.0) * pow(t_, 5) + pow(t_, 3) + t_ + 32.0/35.0) / 16.0);
      update_particles(x_.at(i).first, x_.at(i).second);
      if (print()) {
        std::cout << "ID = " << index << "\t t = " << t_ << "\t X = [" << x_.at(i).first->x_.transpose() << "]" << std::endl;
      }
    }
  }

  void Legendre24() {
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_(0) = (pow(t_, 3) - t_) / 2.0;
      x_.at(i).first->x_(1) = (7 * pow(t_, 5) - 10 * pow(t_, 3) + 3 * t_) / 8.0;
      x_.at(i).first->x_(2) = (14 * pow(t_, 7) - 28 * pow(t_, 5) + 14 * pow(t_, 3)) / 16;
      update_particles(x_.at(i).first, x_.at(i).second);
      if (print()) {
        std::cout << "ID = " << index << "\t t = " << t_ << "\t X = [" << x_.at(i).first->x_.transpose() << "]" << std::endl;
      }
    }
  }

  void Legendre34() {
    static double K2 = 1;
    static double K1 = -1 * (63.0 * 64) / (2 * 628.0);
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_(0) = K1 * ((1.25 * pow(t_, 4) - 1.5 * pow(t_, 2) + 0.25) / 2.0);
      x_.at(i).first->x_(1) = K2 * ((7 * pow(t_, 5) - 10 * pow(t_, 3) + 3 * t_) / 8.0);
      x_.at(i).first->x_(2) = K1 * K2 * (((35.0/9.0) * pow(t_, 9) + (134.0/7.0) * pow(t_, 7) - (160.0/5.0) * pow(t_, 5) - 4 * pow(t_, 3) + 3 * t_ - 628.0/63.0)) / 64.0;
      update_particles(x_.at(i).first, x_.at(i).second);
      if (print()) {
        std::cout << "ID = " << index << "\t t = " << t_ << "\t X = [" << x_.at(i).first->x_.transpose() << "]" << std::endl;
      }
    }
  }

  void Legendre46() {
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i).first->x_(0) = (7 * pow(t_, 5) - 10 * pow(t_, 3) + 3 * t_) / 8.0;
      x_.at(i).first->x_(1) = (33 * pow(t_, 7) - 63 * pow(t_, 5) + 35 * pow(t_, 3) - 5 * t_) / 16.0;
      x_.at(i).first->x_(2) = ((462.0/12.0)*pow(t_, 12) - (1320.0/10)*pow(t_, 10) + (1364.0/8.0)*pow(t_, 8) -(616.0/6.0)*pow(t_, 6)+(110.0/4.0)*pow(t_, 4) - (77 - (616./6.)+(110./4.))) / 128.0;
      update_particles(x_.at(i).first, x_.at(i).second);
      if (print()) {
        std::cout << "ID = " << index << "\t t = " << t_ << "\t X = [" << x_.at(i).first->x_.transpose() << "]" << std::endl;
      }
    }
  }

  bool step() {
    {
      lock_t lock(x_mutex_); 
      t_ += Config::dt();
      if (t_ >= 1)
        return false;
      switch (type_) {
        case InputType::Optimal:
          sinosuidal();
          break;
        case InputType::Legendre13:
          Legendre13();
          break;
        case InputType::Legendre23:
          Legendre23();
          break;
        case InputType::Legendre24:
          Legendre24();
          break;
        case InputType::Legendre34:
          Legendre34();
          break;
        case InputType::Legendre46:
          Legendre46();
          break;
        default:
          std::cerr << "Invalid input type" << std::endl;
          return false;
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
  double u0;
  size_t index;
  double a_;
  double c_;
  double t_={-1};
  bool stop_exec_={false};
  std::mutex x_mutex_;
  std::mutex stop_mutex_;
  InputType type_;
};

class NonHolonomicIntegrator {
public: 
  NonHolonomicIntegrator(vector<double> a, vector<double> c) {
    // particles_.resize(4);
    // size_t i=0;
    // particles_[0] = new NonHolonomicIntegratorSingle(0, a[i], c[i], Vector3d(1, 0, 0), InputType::Legendre22);
    // particles_[1] = new NonHolonomicIntegratorSingle(1, a[i], c[i], Vector3d(0, 1, 0), InputType::Legendre23);
    // particles_[2] = new NonHolonomicIntegratorSingle(2, a[i], c[i], Vector3d(0, 0, 1), InputType::Legendre24);
    // particles_[3] = new NonHolonomicIntegratorSingle(3, a[i], c[i], Vector3d(1, 1, 1), InputType::Legendre34);

    particles_.resize(a.size());
    for (size_t i=0; i<a.size(); ++i) {
      particles_[i] = new NonHolonomicIntegratorSingle(i, a[i], c[i], InputType::Legendre13);
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
    return true;
  }

  inline bool stop() {
    for (size_t i=0; i<particles_.size(); ++i) {
      if (particles_[i]->stop())
        return true;
    }
    return false;
  }

  inline controls::PARTICLE_TRAIL x() {
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
