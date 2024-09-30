#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct VertexData {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
  glm::vec3 tangent;
  glm::vec3 bitangent;
  glm::vec4 color;
};

class Mesh {
private:
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;
  void SetupMesh();
  void CleanupMesh();

protected:
  std::vector<VertexData> vertexData;
  std::vector<unsigned int> indices;

public:
  Mesh() = default;
  Mesh(const std::vector<VertexData> &data,
       const std::vector<unsigned int> &newIndices);

  virtual ~Mesh();

  void SetMeshData(const std::vector<VertexData> &newData,
                   const std::vector<unsigned int> &newIndices);

  void SetupDepthTestMode() const;
  void SetupPolygonMode() const;
  void SetupCullingMode() const;
  void ResetRendererState() const;

  std::vector<VertexData> GetVertexData() const { return vertexData; }

  std::vector<unsigned int> GetIndices() const { return indices; }

  GLuint GetVAO() const { return VAO; }
};
