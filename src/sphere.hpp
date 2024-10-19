#pragma once

#include "shape.hpp"
#include <glm/glm.hpp>

class Sphere : public Shape {
public:
  Sphere(float radius = 1.0f, int sectors = 36, int stacks = 18);

private:
  void generateGeometry();
  float radius;
  int sectors;
  int stacks;
};
