#include "perspective_camera.hpp"

#include "node.hpp"
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/ext.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/string_cast.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <string>

Node::Node(std::string name) : name(name){};

Node::Node(std::shared_ptr<Geometry> geometry,
           std::shared_ptr<Material> material)
    : geometry(geometry), material(material) {
  if (geometry) {
    this->name = geometry->getName();
  }
}

Node::~Node() { std::cout << "Removing Node " << name << "\n"; }

void Node::updateWorldTransform() {
  if (dirty) {
    updateLocalTransform(); // Recalculate local matrix if dirty
    if (auto parent = this->parent.lock()) {
      worldTransform = parent->worldTransform * localTransform;
    } else {
      worldTransform = localTransform;
    }
    dirty = false; // Clear the dirty flag after updating
  }

  // Always traverse to children, whether or not this node was dirty
  for (auto &child : children) {
    child->updateWorldTransform();
  }
}

// TODO: Use quaternions as a substitute to euler angles to save on matrix
// multiplications (and to learn quaternions).
void Node::updateLocalTransform() {
  // Create transformation matrices
  glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), position);
  glm::mat4 rotateMatrixX =
      glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 rotateMatrixY =
      glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotateMatrixZ =
      glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

  // Combine transformations in the correct order: Scale -> Rotate ->
  // Translate
  localTransform = translateMatrix * rotateMatrixY * rotateMatrixX *
                   rotateMatrixZ * scaleMatrix;
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
  dirty = true;
  for (auto &child : children) {
    child->markDirty();
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
}

void Node::setRot(glm::vec3 rotation) {
  markDirty();
  this->rotation = rotation;
}

void Node::setScale(glm::vec3 scale) {
  markDirty();
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

  material->apply(this->worldTransform);
  if (geometry) {
    this->geometry->Draw();
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  for (auto node : this->children) {
    node->draw(camera);
  }
}
void Node::printTransforms(std::string indent) {
  std::cout << indent << "Node: " << name << std::endl;
  std::cout << indent << "Position: " << glm::to_string(position) << std::endl;
  std::cout << indent << "Rotation: " << glm::to_string(rotation) << std::endl;
  std::cout << indent << "Local Matrix: " << glm::to_string(localTransform)
            << std::endl;
  std::cout << indent << "World Matrix: " << glm::to_string(worldTransform)
            << std::endl;

  for (auto &child : children) {
    child->printTransforms(indent + "  ");
  }
}
