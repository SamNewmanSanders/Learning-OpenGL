#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Entity.h"

std::vector<float> generateSphereVertices(unsigned int segments, float radius);
std::vector<unsigned int> generateSphereIndices(unsigned int segments);

std::vector<Entity> sortEntitiesByDistance(
    const std::vector<Entity>& entities,
    const glm::vec3& referencePos);