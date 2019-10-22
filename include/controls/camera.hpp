#ifndef _CONTROLS_CAMERA_HPP_
#define _CONTROLS_CAMERA_HPP_

#include "controls/global.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"

#include <vector>

namespace controls {

enum class camera_movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

enum class cam_type {
  STATIC,
  MOVING
};

// default camera values in degrees
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 1.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM        = 45.0f;

class Camera {
public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;

  float yaw;
  float pitch;

  float movement_speed;
  float mouse_sensitivity;
  float zoom;

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = YAW, float pitch = PITCH)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), 
      movement_speed(SPEED),
      mouse_sensitivity(SENSITIVITY),
      zoom(ZOOM)
  {
    this->world_up = glm::vec3(0.0f, 1.0f, 0.0f);       
    this->position = position;
    this->up       = up;
    this->yaw      = yaw;
    this->pitch    = pitch;

    update_camera_vectors();
  }

  glm::mat4 get_view_matrix(cam_type type=cam_type::STATIC) {
    if (type == cam_type::STATIC)
      return glm::lookAt(position, position + front, up);
    else
      return glm::lookAt(position, glm::vec3(0.0f), up);
  }

  void process_keyboard(camera_movement direction, float delta_time) {
    float velocity = movement_speed * delta_time;
    if (direction == camera_movement::FORWARD)
      position += front * velocity;
    else if (direction == camera_movement::BACKWARD)
      position -= front * velocity;
    else if (direction == camera_movement::RIGHT)
      position += right * velocity;
    else if (direction == camera_movement::LEFT)
      position -= right * velocity;
  }

  void update_position(glm::vec3 position) {
    this->position = position;
  }

  void process_mouse_movement(float dx, float dy) {
    dx *= mouse_sensitivity;
    dy *= mouse_sensitivity;

    yaw += dx;
    pitch += dy;

    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;

    update_camera_vectors();
  }

  void process_scroll(float dy) {
    if (zoom >= 1.0f && zoom <= 45.0f)
      zoom -= dy;
    if (zoom <= 1.0f)
      zoom = 1.0f;
    if (zoom >= 45.0f)
      zoom = 45.0f;
  }

private:
  void update_camera_vectors() {
    glm::vec3 _front;
    _front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    _front.y = sin(glm::radians(pitch));
    _front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    this->front = glm::normalize(_front);

    this->right = glm::normalize(glm::cross(front, world_up));
    this->up    = glm::normalize(glm::cross(this->right, front));
  }
};

} // namespace controls

#endif
