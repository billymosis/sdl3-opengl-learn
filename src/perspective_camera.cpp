#include "perspective_camera.hpp"
#include "SDL3/SDL_log.h"
#include <glm/glm.hpp>

#define _USE_MATH_DEFINES

PerspectiveCamera::PerspectiveCamera(const Window &window) : window(window) {
  this->name = name;
  this->position = position;
  this->rotation = rotation;
  this->scale = scale;
  this->worldTransform = glm::mat4(1.0f);
  this->localTransform = glm::mat4(1.0f);
  this->mouseScroll(0.0f);
}

void PerspectiveCamera::updateWorldTransform() {
  if (dirty) {
    auto parent = this->parent.lock();
    this->updateLocalTransform();
    glm::mat4 parentWorldTransform;
    if (parent.get() != nullptr) {
      parentWorldTransform = parent.get()->worldTransform;
      worldTransform = parent.get()->worldTransform * localTransform;
    } else {
      worldTransform = glm::mat4() * localTransform;
    }

    // Get a normalized rotation vector (the forward vector) that points
    // in the direction the camera is facing that's 1 unit in length.
    // Also generate the up and right vectors from the forward vector.
    const auto decomposed = this->getDecomposedTransform();
    updateView();

    // Adding the rotation results in a point 1 unit in front of the
    // camera which is then passed to the lookAt function to create a
    // view matrix. The projection matrix is pre-baked for performance.
    target = decomposed.translation + forward;
    viewMatrix = glm::lookAt(decomposed.translation, target, up);

    dirty = false;
    for (auto node : children) {
      node.get()->markDirty();
      node.get()->updateWorldTransform();
    }
  } else {
    for (auto node : children) {
      node.get()->updateWorldTransform();
    }
  }
}

glm::vec3 PerspectiveCamera::getFront() { return this->forward; }
glm::vec3 PerspectiveCamera::getRight() { return this->right; }

void PerspectiveCamera::mouseMovement(float xoffset, float yoffset,
                                      bool constrainPitch) {
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch -= yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  // update Front, Right and Up Vectors using the updated Euler angles
  updateView();
}

void PerspectiveCamera::updateView() {
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  forward = glm::normalize(front);
  up = glm::cross(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)), forward);
  right = glm::cross(forward, up);

  // Create new right and up vectors based on the roll.
  right = glm::normalize(glm::cross(forward, up));
  up = glm::cross(right, front);
}

void PerspectiveCamera::mouseScroll(float yoffset) {
  float zoomSpeed = 0.1f;
  float minFOV = glm::radians(10.0f); // Minimum FOV in radians (most zoomed in)
  float maxFOV =
      glm::radians(120.0f); // Maximum FOV in radians (most zoomed out)
  float currentFOV = 2 * atan(1.0f / zoom); // Convert zoom to FOV

  // Adjust FOV based on scroll
  currentFOV *= (1.0f - yoffset * zoomSpeed);

  // Clamp FOV to prevent flipping and overly narrow views
  currentFOV = glm::clamp(currentFOV, minFOV, maxFOV);

  // Convert FOV back to zoom factor
  zoom = 1.0f / tan(currentFOV * 0.5f);

  SDL_Log("fov: %f", currentFOV);
  this->projectionMatrix = glm::perspective(
      currentFOV, (float)window.width / (float)window.height, 0.1f, 100.0f);
}
