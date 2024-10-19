#include "custom_geometry.hpp"

CustomGeometry::CustomGeometry(vector<Vertex> vertices,
                               vector<unsigned int> indices,
                               vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  // now that we have all the required data, set the vertex buffers and its
  // attribute pointers.
  setupMesh();
}

CustomGeometry::CustomGeometry(const Geometry &shape) {
  this->vertices = shape.vertices;
  this->indices = shape.indices;
  this->textures = shape.textures;
  setupMesh();
}

CustomGeometry::CustomGeometry() {}
