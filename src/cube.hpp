#pragma once

#include "shape.hpp"

class Cube : public Geometry {
public:
  Cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
  virtual std::string getName() override;

private:
  void generateGeometry() override;
  float width, height, depth;
};
