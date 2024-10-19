#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>

using namespace std;

class Mesh : public Shape {
public:
  Mesh();
  Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
       vector<Texture> textures);
  Mesh(const Shape& shape);
};
