#include "sphere.hpp"
#include "common.h"

Sphere::Sphere(float radius, int sectors, int stacks)
    : radius(radius), sectors(sectors), stacks(stacks) {
  generateGeometry();
  setupMesh();
}

void Sphere::generateGeometry() {

  float sectorStep = 2 * PI / sectors;
  float stackStep = PI / stacks;
  float sectorAngle, stackAngle;

  for (int i = 0; i <= stacks; ++i) {
    stackAngle = PI / 2 - i * stackStep;
    float xy = radius * cosf(stackAngle);
    float z = radius * sinf(stackAngle);

    for (int j = 0; j <= sectors; ++j) {
      sectorAngle = j * sectorStep;

      float x = xy * cosf(sectorAngle);
      float y = xy * sinf(sectorAngle);

      // Position
      glm::vec3 position(x, y, z);

      // Normal
      glm::vec3 normal = glm::normalize(position);

      // Texture coordinates
      float s = (float)j / sectors;
      float t = (float)i / stacks;
      glm::vec2 texCoord(s, t);

      // Add vertex
      vertices.push_back({position, normal, texCoord});
    }
  }

  // Generate indices
  for (int i = 0; i < stacks; ++i) {
    int k1 = i * (sectors + 1);
    int k2 = k1 + sectors + 1;

    for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
      if (i != 0) {
        indices.push_back(k1);
        indices.push_back(k2);
        indices.push_back(k1 + 1);
      }

      if (i != (stacks - 1)) {
        indices.push_back(k1 + 1);
        indices.push_back(k2);
        indices.push_back(k2 + 1);
      }
    }
  }
}

std::string Sphere::getName() { return "SPHERE"; }
