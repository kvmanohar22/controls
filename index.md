---
layout: post
title: Linear Controls
---

# Linear Controls

Implementation of linear controls as part of **EE60011: Control Theory** course at IIT Kharagpur by [Prof. Sanand](http://www.facweb.iitkgp.ac.in/~sanand/f_sanand.html)

<div class="fig figcenter fighighlight">
  <img src="imgs/out_converge.gif" width=49% style="margin-right:1px;">
  <img src="imgs/diverge.gif" width=49%>
</div> 

**Fig1:** Rendering of evolution of particles with eigenvalues `(-0.43016,2.61428) (-0.43016,-2.61428),(-1.13968, 0)` on the left and `(0.440736,0)  (-1.22037,2.75435) (-1.22037,-2.75435)` on the right. Note that, for the rendering on the left, real part of complex eigenvalues is strictly less than zero and hence (exponentially) stable but for the ones on the right, real part of complex eigenvalues is positive and hence the particles shoot to infinity exponentially fast.
