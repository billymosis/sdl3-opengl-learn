#pragma once

#include "SDL3/SDL_log.h"
#include "shader.hpp"
#include "vertex.hpp"
#include <string>
#include <vector>

class Geometry {
public:
  std::vector<Vertex> vertices = {};
  std::vector<unsigned int> indices = {};
  GLuint VAO{0};
  void Draw();
  virtual std::string getName();
  virtual void generateGeometry() {};
  virtual ~Geometry() {};

protected:
  GLuint VBO{0};
  GLuint EBO{0};
  void setupMesh();
};
