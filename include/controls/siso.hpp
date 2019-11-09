#ifndef _CONTROLS_SISO_H_
#define _CONTROLS_SISO_H_

#include "controls/global.hpp"
#include "controls/config.hpp"

namespace controls {

/*
 *
 * Given
 *    1. {alpha_1, alpha_2, ..., alpha_n}
 *    2. desired eigen values V = {v1, v2, ..., vn}
 * To find the matrix K for which the closed loop system has 
 * the above V set of eigenvalues
 *      _                                    _
 *     | -alpha_1  -alpha_2   ....   -alpha_n |
 *     |    1         0                  0    |
 *     |    0         1                  0    |
 * A = .    .                            .    |
 *     .    .                            .    |
 *     .    .                            .    |
 *     |    0         0                  0    |
 *     
 *
 * B = [1, 0, ..., 0]^T
 *
 */

template <typename T>
class SISO {
public:
  SISO(vector<T>& alphas, vector<T>& roots)
    : n(alphas.size()),
      alphas_(alphas),
      rroots_(roots)
  {
    find_coeffs(); 
    K_.resize(n); 
    findK();
  }

  void findK() {
    for(size_t i=0; i<n; ++i) {
      K_[i] = coeffs_[i+1] + alphas_[i]; 
    }
  }

  void find_coeffs() {
    coeffs_.resize(n+1);
    coeffs_[0] = 1; // since it's a monic polynomial 
    for(size_t i=0; i<n+1; ++i) {
      coeffs_[i+1] = -rroots_[i] * coeffs_[i];
      for(size_t j=i; j >= 1; --j) {
        coeffs_[j] = coeffs_[j] - rroots_[i] * coeffs_[j-1];
      }
    }
  }

  void summary() {
    // Matrix A 
    cout << "Matrix A:\n";
    for(size_t i=0; i<n; ++i) {
      cout << alphas_[i] << '\t';
    }
    cout << '\n';
    for(size_t i=0; i<n-1; ++i) {
      for(size_t j = 0; j<n; ++j) {
        if(i == j)
          cout << 1 << '\t';
        else 
          cout << 0 << '\t';
      }
      cout << '\n';
    }
    
    // Desired roots 
    cout << '\n' << "Desired roots:\n"; 
    for(size_t i=0; i<n; ++i)
      cout << rroots_[i] << '\t';
    cout << endl;

    // Polynomial 
    cout << '\n' << "Polynomial with required roots:\n"; 
    cout << 1 << '\t';
    for(size_t i=1; i<n+1; ++i)
      cout << coeffs_[i] << '\t';
    cout << endl;

    // Required Matrix K
    cout << '\n' << "Vector K:\n"; 
    for(size_t i=0; i<n; ++i)
      cout << K_[i] << '\t';
    cout << endl; 
  }

private:
  size_t n;              // nxn A matrix
  vector<T> alphas_;     // A matrix coeffs in first row; alpha_1, alpha_2, ..., alpha_n
  vector<T> rroots_;     // required roots
  vector<T> coeffs_;     // coeffs of required polynomial; coeff_1 (constant), coeff_2(s), ..., coeff_n (s^{n-1})
  vector<T> K_;          // matrix to asymptotically stabilize the system
};

}

#endif

