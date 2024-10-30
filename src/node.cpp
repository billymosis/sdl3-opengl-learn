#include "SDL3/SDL_log.h"
#include "glm/ext.hpp"
#include "glm/fwd.hpp"
#include "perspective_camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "node.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <string>

Node::Node(std::string name) : name(name) {};

Node::Node(std::shared_ptr<Geometry> geometry,
           std::shared_ptr<Material> material)
    : geometry(geometry), material(material) {
  if (geometry) {
    this->name = geometry->getName();
  }
  this->position = glm::vec3(0.0f);
  this->rotation = glm::vec3(0.0f);
  this->scale = glm::vec3(1.0f);
  this->worldTransform = glm::mat4(1.0f);
  this->localTransform = glm::mat4(1.0f);
}

Node::~Node() { std::cout << "Removing Node " << name << "\n"; }

void Node::updateWorldTransform() {
  if (dirty) {
    auto parent = this->parent.lock();
    this->updateLocalTransform();
    if (parent.get() != nullptr) {
      worldTransform = parent.get()->worldTransform * localTransform;
    } else {
      worldTransform = localTransform;
    }

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

// TODO: Use quaternions as a substitute to euler angles to save on matrix
// multiplications (and to learn quaternions).
void Node::updateLocalTransform() {
  if (dirty) {
    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, rotation.y, glm::vec3(0.0, 1.0, 0.0));
    transform = glm::rotate(transform, rotation.x, glm::vec3(1.0, 0.0, 0.0));
    transform = glm::rotate(transform, rotation.z, glm::vec3(0.0, 0.0, 1.0));
    transform = glm::scale(transform, scale);
    localTransform = transform;
  }
}

void Node::add(std::shared_ptr<Node> node) {
  if (node) {
    node.get()->parent = shared_from_this();
    node.get()->dirty = true;
    children.push_back(node);
  }
}

void Node::remove() {
  auto parent = this->parent.lock();
  if (parent.get() == nullptr) {
    std::cout << "Attempted to remove " << name << " but it has no parent"
              << "\n";
    return;
  }

  auto it =
      std::find_if(parent.get()->children.begin(), parent.get()->children.end(),
                   [this](auto i) { return i.get() == this; });
  if (it != parent.get()->children.end()) {
    parent.get()->children.erase(it); // Order matters so no swap.
  }
}

void Node::markDirty() {
  if (!dirty) {
    dirty = true;
    for (auto &child : children) {
      child->markDirty();
    }
  }
}

DecomposedTransform Node::getDecomposedTransform() {
  DecomposedTransform decomposed;
  glm::decompose(worldTransform, decomposed.scale, decomposed.rotation,
                 decomposed.translation, decomposed.skew,
                 decomposed.perspective);
  return decomposed;
}

glm::vec3 Node::getPos() const { return position; }

glm::vec3 Node::getRot() const { return rotation; }

glm::vec3 Node::getScale() const { return scale; }

void Node::setPos(glm::vec3 position) {
  markDirty();
  this->position = position;
  updateLocalTransform();
}

void Node::setRot(glm::vec3 rotation) {
  markDirty();
  this->rotation = rotation;
  updateLocalTransform();
}

void Node::setScale(glm::vec3 scale) {
  dirty = true;
  this->scale = scale;
}

void Node::setUBOProgram(GLuint matricesUBO) {
  this->matricesUBO = matricesUBO;
}

void Node::draw(PerspectiveCamera *camera) {
  if (!material)
    return;
  auto shaderProgram{material->getShader().ID};
  GLuint matricesBlockIndex = glGetUniformBlockIndex(shaderProgram, "Matrices");
  glUniformBlockBinding(shaderProgram, matricesBlockIndex, 0);
  glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(camera->projectionMatrix));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(camera->viewMatrix));

  this->material->getShader().setMat4("model", this->worldTransform);
  // this->material->setColor(glm::vec4(1.0f,0.0f,0.0f,1.0f));

  material->apply();
  if (geometry) {
    this->geometry->Draw();
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  for (auto node : this->children) {
    node->draw(camera);
  }
}
