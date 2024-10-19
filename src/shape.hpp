#pragma once

#include "shader.hpp"
#include "vertex.hpp"
#include <string>
#include <vector>

class Geometry {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int VAO;
  void Draw(Shader &shader);
  virtual std::string getName();
  virtual void generateGeometry() {};

protected:
  unsigned int VBO, EBO;
  void setupMesh();
};
