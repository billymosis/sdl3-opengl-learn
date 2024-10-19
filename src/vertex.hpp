#pragma once

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent = glm::vec3(0.0f);
  // bitangent
  glm::vec3 Bitangent = glm::vec3(0.0f);
  // bone indexes which will influence this vertex
  int m_BoneIDs[MAX_BONE_INFLUENCE] = {};
  // weights from each bone
  float m_Weights[MAX_BONE_INFLUENCE] = {};
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};
