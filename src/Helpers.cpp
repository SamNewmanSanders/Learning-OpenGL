#include "Helpers.h"
#include <cmath>

std::vector<float> generateSphereVertices(unsigned int segments, float radius) {
    std::vector<float> vertices;
    const float PI = 3.14159265359f;

    for (unsigned int y = 0; y <= segments; ++y) {
        for (unsigned int x = 0; x <= segments; ++x) {

            float xSegment = (float)x / (float)segments;
            float ySegment = (float)y / (float)segments;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            // Position (scaled by radius)
            vertices.push_back(xPos * radius);
            vertices.push_back(yPos * radius);
            vertices.push_back(zPos * radius);

            // Normal (unit vector, not scaled)
            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            // UV coordinates
            vertices.push_back(xSegment);
            vertices.push_back(ySegment);
        }
    }

    return vertices;
}

std::vector<unsigned int> generateSphereIndices(unsigned int segments) {
    std::vector<unsigned int> indices;

    for (unsigned int y = 0; y < segments; ++y) {
        for (unsigned int x = 0; x < segments; ++x) {
            unsigned int i0 = y * (segments + 1) + x;
            unsigned int i1 = i0 + segments + 1;
            unsigned int i2 = i0 + 1;
            unsigned int i3 = i1 + 1;

            indices.push_back(i0);
            indices.push_back(i1);
            indices.push_back(i2);

            indices.push_back(i2);
            indices.push_back(i1);
            indices.push_back(i3);
        }
    }

    return indices;
}
