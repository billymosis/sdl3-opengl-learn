#include "cube.hpp"

Cube::Cube(float width, float height, float depth)
    : width(width), height(height), depth(depth) {
  generateGeometry();
  setupMesh();
}

void Cube::generateGeometry() {
  // Half sizes for easier calculations
  float w = width / 2.0f;
  float h = height / 2.0f;
  float d = depth / 2.0f;

  // Define the 8 corners of the cube
  glm::vec3 positions[8] = {
      {-w, -h, -d}, {w, -h, -d}, {w, h, -d}, {-w, h, -d}, // back face
      {-w, -h, d},  {w, -h, d},  {w, h, d},  {-w, h, d}   // front face
  };

  // Define the cube's vertex data with normals and texture coordinates
  // (optional)
  vertices = {
      // Back face
      {positions[0], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
      {positions[1], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
      {positions[2], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
      {positions[3], glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},

      // Front face
      {positions[4], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
      {positions[5], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
      {positions[6], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
      {positions[7], glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

      // Left face
      {positions[0], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {positions[3], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {positions[7], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {positions[4], glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

      // Right face
      {positions[1], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {positions[2], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {positions[6], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {positions[5], glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

      // Top face
      {positions[3], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {positions[2], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {positions[6], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {positions[7], glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},

      // Bottom face
      {positions[0], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {positions[1], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {positions[5], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
      {positions[4], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
  };

  // Indices for the cube (12 triangles)
  indices = {// Back face
             0, 1, 2, 0, 2, 3,
             // Front face
             4, 5, 6, 4, 6, 7,
             // Left face
             8, 9, 10, 8, 10, 11,
             // Right face
             12, 13, 14, 12, 14, 15,
             // Top face
             16, 17, 18, 16, 18, 19,
             // Bottom face
             20, 21, 22, 20, 22, 23};
}

std::string Cube::getName() { return "CUBE"; }
