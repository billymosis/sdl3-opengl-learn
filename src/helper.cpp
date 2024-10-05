#include "helper.hpp"
#include "mesh.hpp"

void GenerateTangentsBitangents(std::vector<VertexData> &vertexData,
                                const std::vector<unsigned int> &indices) {
  for (size_t i = 0; i < indices.size(); i += 3) {
    VertexData &vert1 = vertexData[indices[i]];
    VertexData &vert2 = vertexData[indices[i + 1]];
    VertexData &vert3 = vertexData[indices[i + 2]];

    const glm::vec3 dPos1 = vert2.position - vert1.position;
    const glm::vec3 dPos2 = vert3.position - vert1.position;
    const glm::vec2 dUV1 = vert2.uv - vert1.uv;
    const glm::vec2 dUV2 = vert3.uv - vert1.uv;

    const float invDet = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);

    glm::vec3 tangent = invDet * (dUV2.y * dPos1 - dUV1.y * dPos2);
    glm::vec3 bitangent = invDet * (dUV1.x * dPos2 - dUV2.x * dPos1);

    tangent = glm::normalize(tangent);
    bitangent = glm::normalize(bitangent);

    vert1.tangent = tangent;
    vert1.bitangent = bitangent;
    vert2.tangent = tangent;
    vert2.bitangent = bitangent;
    vert3.tangent = tangent;
    vert3.bitangent = bitangent;
  }
}

void GenerateCube(Mesh *targetMesh) {
  std::vector<VertexData> vertexData;
  std::vector<unsigned int> indices;

  //////// Generate vertex data
  VertexData data;

  // Front
  data.position = glm::vec3(-0.5f, -0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  data.uv = glm::vec2(0.25f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, -0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  data.uv = glm::vec2(0.25f, 0.33f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, 0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  data.uv = glm::vec2(0.0f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, 0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  data.uv = glm::vec2(0.0f, 0.33f);

  vertexData.push_back(data);

  // Right
  data.position = glm::vec3(0.5f, -0.5f, -0.5f);
  data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.25f, 0.33f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, -0.5f, 0.5f);
  data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.5f, 0.33f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, 0.5f, -0.5f);
  data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.25f, 0.0f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, 0.5f, 0.5f);
  data.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.5f, 0.0f);

  vertexData.push_back(data);

  // Back
  data.position = glm::vec3(0.5f, -0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  data.uv = glm::vec2(0.5f, 0.33f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, -0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  data.uv = glm::vec2(0.5f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, 0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  data.uv = glm::vec2(0.75f, 0.33f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, 0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  data.uv = glm::vec2(0.75f, 0.66f);

  vertexData.push_back(data);

  // Left
  data.position = glm::vec3(-0.5f, -0.5f, 0.5f);
  data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.5f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, -0.5f, -0.5f);
  data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.25f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, 0.5f, 0.5f);
  data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.5f, 1.0f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, 0.5f, -0.5f);
  data.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  data.uv = glm::vec2(0.25f, 1.0f);

  vertexData.push_back(data);

  // Bottom
  data.position = glm::vec3(-0.5f, -0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  data.uv = glm::vec2(0.5f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, -0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  data.uv = glm::vec2(0.5f, 0.33f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, -0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  data.uv = glm::vec2(0.25f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, -0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  data.uv = glm::vec2(0.25f, 0.33f);

  vertexData.push_back(data);

  // Top
  data.position = glm::vec3(-0.5f, 0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  data.uv = glm::vec2(1.0f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, 0.5f, -0.5f);
  data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  data.uv = glm::vec2(1.0f, 0.33f);

  vertexData.push_back(data);

  data.position = glm::vec3(-0.5f, 0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  data.uv = glm::vec2(0.75f, 0.66f);

  vertexData.push_back(data);

  data.position = glm::vec3(0.5f, 0.5f, 0.5f);
  data.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  data.uv = glm::vec2(0.75f, 0.33f);

  vertexData.push_back(data);
  ////////

  // Generate indices
  for (int i = 0; i < 6; i++) {
    indices.push_back(0 + i * 4);
    indices.push_back(2 + i * 4);
    indices.push_back(1 + i * 4);
    indices.push_back(3 + i * 4);
    indices.push_back(1 + i * 4);
    indices.push_back(2 + i * 4);
  }

  GenerateTangentsBitangents(vertexData, indices);
  targetMesh->SetMeshData(vertexData, indices);
}
