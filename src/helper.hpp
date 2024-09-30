#pragma once

#include "mesh.hpp"
#include <vector>
#include <memory>

void GenerateTangentsBitangents(std::vector<VertexData> &vertexData,
                                const std::vector<unsigned int> &indices);

void GenerateCube(const std::shared_ptr<Mesh> &targetMesh);
