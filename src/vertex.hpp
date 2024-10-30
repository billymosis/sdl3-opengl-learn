#pragma once

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  glm::vec3 Position{0.0f};
  glm::vec3 Normal{0.0f};
  glm::vec2 TexCoords{0.0f};
  glm::vec3 Tangent{0.0f};
  glm::vec3 Bitangent{0.0f};
  glm::ivec4 m_BoneIDs{0}; // Initialize to invalid bone ID
  glm::vec4 m_Weights{0.0f};
};
