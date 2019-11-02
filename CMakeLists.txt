cmake_minimum_required(VERSION 3.3)
project(controls)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_VERBOSE_MAKEFILE OFF)
SET(CMAKE_CXX_FLAGS "-Wall -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unknown-pragmas -Wpedantic")
SET(HEADERS include)

find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(PROJECT_HEADERS include)
include_directories(
  ${PROJECT_HEADERS}
  ${EIGEN3_INCLUDE_DIRS}
  include/controls/ext
)

list(APPEND LIBS
  glfw
)

list(APPEND SRCS
  src/renderer.cpp
  src/shader.cpp
  src/window.cpp
  src/config.cpp
)

add_library(controls SHARED
  ${SRCS}
  src/glw3.c
)
target_link_libraries(controls ${LIBS})
target_include_directories(controls PRIVATE ${PROJECT_HEADERS}/controls)

add_executable(test_linear test/test_linear.cpp)
target_link_libraries(test_linear controls)

