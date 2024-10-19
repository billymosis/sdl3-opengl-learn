#pragma once

#include "shader.hpp"
#include "vertex.hpp"
#include <vector>

class Shape {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int VAO;
  virtual void generateGeometry() {};
  void Draw(Shader &shader);

protected:
  unsigned int VBO, EBO;
  void setupMesh();
};
