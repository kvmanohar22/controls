---
title: Linear Controls
---

<script src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.0/MathJax.js?config=TeX-AMS-MML_HTMLorMML" type="text/javascript"></script>

# Linear Controls

## Contents
- [Linear systems](#linear)
  - [Complex eigenvalues with zero real part](#lin_zero)
  - [Complex eigenvalues with positive real part](#lin_pos)
  - [Complex eigenvalues with negative real part](#lin_neg)
- [Controllable systems](#controllable)
- [Uncontrollable systems](#uncontrollable)
  - [Stabilizable](#stabilizable)
  - [Non-stabilizable](#non_stabilizable)

<a name='linear'></a>
## Linear Systems
The systems considered are continuous linear time invariant autonomous systems.

$$
\frac{d}{dt} x(t) = Ax(t)
$$

The solution to the above system being,

$$
x(t) = e^{At}x(t_0)
$$

In what follows, I present systems with different initial conditions i.e, \\(x(t_0)\\) for various eigenvalues.

### Marginally stable system
$$
A = 
\begin{bmatrix}
  -2 & 0 & 0 \\ 
   0 & 0 & -2 \\ 
   0 & 2 & 0 \\ 
\end{bmatrix}
$$

The above system has eigenvalues `(-2,0), (0,2), (0,-2)`. Because of real part being zero, \\(\lvert \lvert x(t)\rvert \rvert \\) is bounded.

![convergence](imgs/marginally_stable.gif)
**Fig1:** Marginally stable system.

![convergence](imgs/out_converge.gif)
**Fig2:** Rendering of evolution of particles with eigenvalues `(-0.43016,2.61428) (-0.43016,-2.61428),(-1.13968, 0)`. Real part of complex eigenvalues is strictly less than zero and hence (exponentially) stable.

![divergence](imgs/diverge.gif)
**Fig3:** Rendering of evolution of particles with eigenvalues `(0.440736,0)  (-1.22037,2.75435) (-1.22037,-2.75435)`. Real part of complex eigenvalues is positive and hence the particles shoot to infinity exponentially fast.

