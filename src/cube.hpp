#pragma once

#include "shape.hpp"

class Cube : public Shape {
public:
  Cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

private:
  void generateGeometry();
  float width, height, depth;
};
