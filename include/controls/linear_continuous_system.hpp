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
    A_ = Matrix3d::Identity();
  }

  CLTIS(Matrix3d A)
    : A_(A)
  {}

  CLTIS(Matrix3d A, vector<Vector3d> x)
    : A_(A),
      x_(x)
  {}

 ~CLTIS() =default;

  vector<Vector3d> step() {
    t_ += controls::dt;
    Matrix3d A_exp = (A_*t_).exp();
    for(size_t i=0; i<x_.size(); ++i) {
      x_.at(i) = A_exp * x_.at(i);
    }
    return x_;
  }

  void summary() {
    cout << "State Transition Matrix = \n" << A_ << endl;
    cout << "Eigen values = " << A_.eigenvalues().transpose() << endl;
  }

  inline void initialize(vector<Vector3d>& pos) {
    x_.insert(x_.begin(), pos.begin(), pos.end());
  }

  inline Matrix3d         A() { return A_; }
  inline vector<Vector3d> x() { return x_; }
  inline double           t() { return t_; }

private:
  Matrix3d A_;
  vector<Vector3d> x_;
  double t_={0};
};

} // namespace controls

#endif

