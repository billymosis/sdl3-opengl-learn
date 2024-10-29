#pragma once
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "material.hpp"
#include "shape.hpp"
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <optional>
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

class PerspectiveCamera;

class Node : public std::enable_shared_from_this<Node> {
public:
  std::weak_ptr<Node> parent;
  std::vector<std::shared_ptr<Node>> children;

  GLuint matricesUBO;

  std::shared_ptr<Geometry> geometry;
  std::shared_ptr<Material> material;

  glm::mat4 worldTransform;
  glm::mat4 localTransform;

  std::string name = "BASE_NODE";

  Node(std::string name);
  Node(std::shared_ptr<Geometry> geometry = nullptr,
       std::shared_ptr<Material> material = std::make_shared<Material>(
           "../shaders/model_loading.vert", "../shaders/model_loading.frag"));

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

  void setUBOProgram(GLuint matricesUBO);

  void draw(PerspectiveCamera *camera);

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
