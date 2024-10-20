#pragma once
#include "glm/fwd.hpp"
#include "shape.hpp"
#include "texture.hpp"
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <string>
#include <vector>

static const glm::vec3 VEC3_ZERO(0, 0, 0);
static const glm::vec3 VEC3_ONE(1, 1, 1);

struct DecomposedTransform {
  glm::vec3 scale;
  glm::quat rotation;
  glm::vec3 translation;
  glm::vec3 skew;
  glm::vec4 perspective;
};

class Node : public std::enable_shared_from_this<Node> {
public:
  std::weak_ptr<Node> parent;
  std::vector<std::shared_ptr<Node>> children;

  std::shared_ptr<Geometry> geometry = nullptr;
  std::shared_ptr<Texture> texture = nullptr;

  glm::mat4 worldTransform;
  glm::mat4 localTransform;

  std::string name = "BASE_NODE";

  Node(std::string name);
  Node(std::shared_ptr<Geometry> geometry = nullptr,
       std::shared_ptr<Texture> texture = nullptr);

  virtual ~Node();
  void add(std::shared_ptr<Node> node);
  void remove();

  virtual void updateWorldTransform();

  // Recreates the local-space transform based on pos, rot, and scale.
  virtual void updateLocalTransform();

  DecomposedTransform getDecomposedTransform();

  // Returns the node's position in local-space.
  glm::vec3 getPos() const;

  // Returns the node's rotation in local-space.
  glm::vec3 getRot() const;

  // Returns the node's scale in local-space.
  glm::vec3 getScale() const;

  // Sets the node's position in local-space.
  void setPos(glm::vec3 position);

  // Sets the node's rotation in local-space.
  void setRot(glm::vec3 rotation);

  // Sets the node's scale in local-space.
  void setScale(glm::vec3 scale);

  // Sets the diry flag for the node so transforms are updated later.
  void markDirty();

  void draw(Shader &shader);

protected:
  // Position of the node in local-space.
  glm::vec3 position;

  // Rotation of the node in local-space.
  glm::vec3 rotation;

  // Scale of the node in local-space.
  glm::vec3 scale;

  // Dirty flag used to speed up tree traversal and prevent cyclic loops.
  // This flag will be set when the transform is changed.
  bool dirty = true;
};
