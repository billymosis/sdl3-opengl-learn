#include "mesh.hpp"

Mesh::Mesh(const std::vector<VertexData> &data,
           const std::vector<unsigned int> &newIndices) {
  this->vertexData = data;
  this->indices = newIndices;
  this->SetupMesh();
}

Mesh::~Mesh() { this->CleanupMesh(); }

void Mesh::SetMeshData(const std::vector<VertexData> &newData,
                       const std::vector<unsigned int> &newIndices) {
  this->CleanupMesh();

  this->vertexData.clear();
  this->indices.clear();

  this->vertexData = newData;
  this->indices = newIndices;

  this->SetupMesh();
}

void Mesh::SetupMesh() {
  GLuint aPos = 0;
  GLuint aNormal = 1;
  GLuint aTexCoord = 2;
  GLuint aTangent = 3;
  GLuint aBitangent = 4;
  GLuint aColor = 5;

  glCreateVertexArrays(1, &this->VAO);
  glCreateBuffers(1, &this->VBO);
  glCreateBuffers(1, &this->EBO);

  glNamedBufferData(VBO, vertexData.size() * sizeof(VertexData),
                    vertexData.data(), GL_STATIC_DRAW);

  glNamedBufferData(EBO, indices.size() * sizeof(GLuint), indices.data(),
                    GL_STATIC_DRAW);

  glVertexArrayElementBuffer(VAO, EBO);

  GLuint vaoBindingPoint = 0;
  glEnableVertexArrayAttrib(VAO, aPos);
  glVertexArrayAttribBinding(VAO, aPos, vaoBindingPoint);
  glVertexArrayAttribFormat(VAO, aPos, 3, GL_FLOAT, GL_FALSE, 0);

  glEnableVertexArrayAttrib(VAO, aNormal);
  glVertexArrayAttribBinding(VAO, aNormal, vaoBindingPoint);
  glVertexArrayAttribFormat(VAO, aNormal, 3, GL_FLOAT, GL_FALSE,
                            offsetof(VertexData, normal));

  glEnableVertexArrayAttrib(VAO, aTexCoord);
  glVertexArrayAttribBinding(VAO, aTexCoord, vaoBindingPoint);
  glVertexArrayAttribFormat(VAO, aTexCoord, 2, GL_FLOAT, GL_FALSE,
                            offsetof(VertexData, uv));

  glEnableVertexArrayAttrib(VAO, aTangent);
  glVertexArrayAttribBinding(VAO, aTangent, vaoBindingPoint);
  glVertexArrayAttribFormat(VAO, aTangent, 3, GL_FLOAT, GL_FALSE,
                            offsetof(VertexData, tangent));

  glEnableVertexArrayAttrib(VAO, aBitangent);
  glVertexArrayAttribBinding(VAO, aBitangent, vaoBindingPoint);
  glVertexArrayAttribFormat(VAO, aBitangent, 3, GL_FLOAT, GL_FALSE,
                            offsetof(VertexData, bitangent));

  glEnableVertexArrayAttrib(VAO, aColor);
  glVertexArrayAttribBinding(VAO, aColor, vaoBindingPoint);
  glVertexArrayAttribFormat(VAO, aColor, 3, GL_FLOAT, GL_FALSE,
                            offsetof(VertexData, color));

  glVertexArrayVertexBuffer(VAO, vaoBindingPoint, VBO, 0, sizeof(VertexData));
}

void Mesh::CleanupMesh() {
  if (glIsVertexArray(VAO)) {
    glDeleteVertexArrays(1, &VAO);
  }
  if (glIsBuffer(VBO)) {
    glDeleteBuffers(1, &VBO);
  }
  if (glIsBuffer(EBO)) {
    glDeleteBuffers(1, &EBO);
  }
}
