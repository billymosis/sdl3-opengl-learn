#include "custom_geometry.hpp"

CustomGeometry::CustomGeometry(vector<Vertex> vertices,
                               vector<unsigned int> indices) {
  this->vertices = vertices;
  this->indices = indices;

  // now that we have all the required data, set the vertex buffers and its
  // attribute pointers.
  setupMesh();
}

CustomGeometry::CustomGeometry(const Geometry &shape) {
  this->vertices = shape.vertices;
  this->indices = shape.indices;
  setupMesh();
}

CustomGeometry::CustomGeometry() {}
