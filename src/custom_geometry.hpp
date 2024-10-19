#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shape.hpp"
#include "vertex.hpp"
#include <vector>

using namespace std;

class CustomGeometry : public Geometry {
public:
  CustomGeometry();
  CustomGeometry(vector<Vertex> vertices, vector<unsigned int> indices,
                 vector<Texture> textures);
  CustomGeometry(const Geometry &shape);
};
