#pragma once

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <list>

class Entity {

private:
  Entity *parent;
  std::list<Entity *> children;

public:
  Entity(const glm::vec3 &pos = glm::vec4(), const glm::vec3 &ang = glm::vec3(),
         const std::list<Entity *> &children = std::list<Entity *>());
  virtual ~Entity();

  void setPosition(float x, float y, float z);
  void setOrientation(float pitch, float yaw, float roll);

  void move(const glm::vec3 &mov);
  void rotate(const glm::vec3 &rot);
  void rotate(float d_pitch, float d_yaw, float d_roll);

  const glm::vec4 &position(void) const;
  const glm::vec4 &orientation(void) const;

  // NOTE: Idk what author intend to
  void absoluteVectors(glm::vec3 &pos, glm::vec3 &ang) const;

  bool attach(Entity &ent);
  void detach(Entity &ent);

  const std::list<Entity *> &getChildren(void) const;

  const Entity* getParent(void) const;

  // TODO:
  // virtual void render(Renderer &renderer) const;

protected:
  glm::vec3 pos;
  glm::vec3 ang;
  virtual void setParent(Entity &parent_ent);
  void setParent(std::nullptr_t);
};
