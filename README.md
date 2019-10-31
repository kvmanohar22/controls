# Linear Controls

:construction: Work in Progress :construction:

Implementation of linear controls as part of **EE60011: Control Theory** course at IIT Kharagpur by [Prof. Sanand](http://www.facweb.iitkgp.ac.in/~sanand/f_sanand.html)

<p align="center"><img width="70%" height="70%" src="imgs/renderer.png"/></p>

## TODO
- [x] 3D rendering using OpenGL
- [x] Continuous/Discrete Linear Time Invariant Systems
- [x] Particle rendering with different initial conditions
- [x] Rendering particles lifetime
- [ ] Efficient rendering of particles [**use instance buffers**]
- [ ] Continuous/Discrete Linear Time Variant Systems
- [ ] Non-linear systems (pendulum/inverted pendulum as simple examples)
- [ ] MIMO pole placement
- [ ] State evolution of controllable/un-controllable observable/un-observable systems
- [ ] Static/Dynamic output/state feedback systems

## Requirements

- c++14
- OpenGL 3.3+
- Eigen3
- glfw3
- gl3w

### Build

**Note**: Built in Debug mode, to change to Release, check `CMakeLists.txt`

```bash
git clone git@github.com:kvmanohar22/controls.git
cd controls
mkdir build
cd build
cmake ..
```

### Test linear controller

```bash
make test_linear
./test_linear
```
