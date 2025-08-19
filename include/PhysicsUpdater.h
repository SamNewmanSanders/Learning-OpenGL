#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "Utils/fpsTracker.h"
#include "Entity.h"

class PhysicsUpdater
{
    
public:
    PhysicsUpdater(float boxSize, float sphereRadius, float wallThickness, float cellSize);

    void update(float simDeltaTime, std::vector<Entity>& dynamicEntities);

private:

    void applyBoundaries(float simDeltaTime, std::vector<Entity>& dynamicEntities);

    void setCellSize(float cellSize) {spatialHashGrid.cellSize = cellSize;} // If cellSize is dynamically updated

    float boxSize;
    float sphereRadius; 
    float wallThickness;

    // Hash function - primes act as decorelators and XOR evenly distributes.
    struct MyVec3Hash {
    std::size_t operator()(const glm::ivec3& v) const noexcept {
        // Large primes for mixing (reduce collisions)
        const int p1 = 73856093;
        const int p2 = 19349663;
        const int p3 = 83492791;

        return static_cast<std::size_t>(v.x * p1 ^ v.y * p2 ^ v.z * p3);
        }
    };

    struct SpatialHashGrid {
        float cellSize;
        // Relate each grid coordinate to a vector of entities
        std::unordered_map<glm::ivec3 ,std::vector<size_t>, MyVec3Hash> gridCells; 
        // Use size_t as lookup index for entities vector to avoid expensive copying

        SpatialHashGrid(float cellsize) : cellSize(cellsize) {}
        void clear();
        void insert(const glm::vec3& pos, size_t entityIndex);
        const std::vector<size_t> query(const glm::vec3& pos);
    };

    SpatialHashGrid spatialHashGrid;

};