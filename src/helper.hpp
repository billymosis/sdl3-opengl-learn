#pragma once

#include "mesh.hpp"
#include <vector>

void GenerateTangentsBitangents(std::vector<VertexData> &vertexData,
                                const std::vector<unsigned int> &indices);

void GenerateCube(Mesh *targetMesh);
