#ifndef _CONTROLS_LINEAR_H_
#define _CONTROLS_LINEAR_H_

#include "controls/global.hpp"

namespace controls {

/*
 * Continuous Linear Time Invariant System
 * x' = Ax
 *
 * Solution to the above system;
 * x(t) = e^(At)x(0)
 *
 */

class CLTIS {
public: 
  CLTIS() {
    A_ = Eigen::Matrix3d::Identity();
    x_ = Eigen::Vector3d();
  }

  CLTIS(Eigen::Matrix3d A)
    : A_(A),
      x_(Eigen::Vector3d())
  {}

  CLTIS(Eigen::Matrix3d A, Eigen::Vector3d x)
    : A_(A),
      x_(x)
  {}

 ~CLTIS() =default;

  Eigen::Vector3d step() {
    t_ += controls::dt;
    x_ = (A_*t_).exp() * x_;
    return x_;
  }

  void summary() {
    cout << "State Transition Matrix = \n" << A_ << endl;
    cout << "Eigen values = " << A_.eigenvalues().transpose() << endl;
  }

  inline Eigen::Matrix3d A() { return A_; }
  inline Eigen::Vector3d x() { return x_; }
  inline double          t() { return t_; }

private:
  Eigen::Matrix3d A_;
  Eigen::Vector3d x_;
  double t_={0};
};

} // namespace controls

#endif

