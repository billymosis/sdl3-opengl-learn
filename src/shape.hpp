#pragma once

#include "SDL3/SDL_log.h"
#include "shader.hpp"
#include "vertex.hpp"
#include <string>
#include <vector>

class Geometry {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int VAO;
  void Draw();
  virtual std::string getName();
  virtual void generateGeometry() {};
  virtual ~Geometry() {};

protected:
  unsigned int VBO, EBO;
  void setupMesh();
};
