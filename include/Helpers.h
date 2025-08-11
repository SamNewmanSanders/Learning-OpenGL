#pragma once
#include <vector>
#include <glm/glm.hpp>

std::vector<float> generateSphereVertices(unsigned int segments, float radius);
std::vector<unsigned int> generateSphereIndices(unsigned int segments);
