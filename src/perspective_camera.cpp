#include "perspective_camera.hpp"
#include "glm/trigonometric.hpp"
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
    zoom -= (float)yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f; 

  this->projectionMatrix = glm::perspective(
      glm::radians(zoom), (float)window.width / (float)window.height, 0.1f, 100.0f);
}
