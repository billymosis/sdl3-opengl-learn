#pragma once

#include "shape.hpp"
#include <glm/glm.hpp>

class Sphere : public Geometry {
public:
  Sphere(float radius = 1.0f, int sectors = 36, int stacks = 18);
  std::string getName() override;

private:
  void generateGeometry() override;
  float radius;
  int sectors;
  int stacks;
};
