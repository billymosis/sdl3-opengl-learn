#pragma once

#include "node.hpp"
#include "window.hpp"

class PerspectiveCamera : public Node {
public:
  // Projection matrix for the camera.
  glm::mat4 projectionMatrix;

  // View matrix for the camera.
  glm::mat4 viewMatrix;
  Window window;

  PerspectiveCamera(const Window &window);

  // Traverses the tree and updates child transformations, and in addition
  // the view projection matrix as well.
  virtual void updateWorldTransform();

  glm::vec3 getFront();
  glm::vec3 getRight();

  void mouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

  void mouseScroll(float yoffset);
  void updateView();

private:
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
  glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::vec3 target = glm::vec3(0.0f, 0.0f, 1.0f);

  float yaw = -90.0f;
  float pitch = 0.0f;
  // camera options
  float movementSpeed = 1.0f;
  float mouseSensitivity = 0.1f;
  float zoom = 1.0f;
};
