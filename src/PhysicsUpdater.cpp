#include "PhysicsUpdater.h"

PhysicsUpdater::PhysicsUpdater(float boxSize, float sphereRadius, float wallThickness, float cellSize) 
: boxSize(boxSize), sphereRadius(sphereRadius), wallThickness(wallThickness), spatialHashGrid(cellSize)
{}

void PhysicsUpdater::update(float simDeltaTime, std::vector<Entity>& dynamicEntities) {
    
    applyBoundaries(simDeltaTime, dynamicEntities); // Applies gravity and wall bounces


    // Set up a new hash grid each frame (quicker and easier than updating an existing one?)
    spatialHashGrid.clear();
    for(size_t i = 0; i<dynamicEntities.size(); i++)
    {
        spatialHashGrid.insert(dynamicEntities[i].getPosition(), i);
    }

    for (size_t i = 0; i < dynamicEntities.size(); ++i)
    {
        glm::ivec3 cellCoord = glm::floor(dynamicEntities[i].getPosition() / spatialHashGrid.cellSize);

        for (int dx=-1; dx < 2; dx++)
        {
            for (int dy=-1; dy < 2; dy++)
            {
                for (int dz=-1; dz < 2; dz++)
                {
                    glm::ivec3 neighbourCell = cellCoord + glm::ivec3(dx, dy, dz); // Note neighbourcell can also be the current cell here

                    auto it = spatialHashGrid.gridCells.find(neighbourCell);
                    if (it == spatialHashGrid.gridCells.end()) continue; 

                    for (auto& j : it->second)
                    {
                        if (j <= i) continue; // Skip self comparison

                        auto& a = dynamicEntities[i];
                        auto& b = dynamicEntities[j];

                        glm::vec3 posA = a.getPosition();
                        glm::vec3 posB = b.getPosition();
                        glm::vec3 velA = a.getVelocity();
                        glm::vec3 velB = b.getVelocity();
                        float rA = sphereRadius;
                        float rB = sphereRadius;

                        glm::vec3 delta = posB - posA;
                        float dist = glm::length(delta);
                        float minDist = rA + rB;

                        if (dist < minDist && dist > 0.0f) {
                            glm::vec3 normal = delta / dist;

                            // Relative velocity (B relative to A)
                            glm::vec3 relVel = velB - velA;
                            float velAlongNormal = glm::dot(relVel, normal);

                            if (velAlongNormal < 0.0f) {
                                float restitution = 1.0f;
                                float invMassA = 1.0f;
                                float invMassB = 1.0f;

                                float impulseMag = -(1.0f + restitution) * velAlongNormal
                                                / (invMassA + invMassB);

                                glm::vec3 impulse = impulseMag * normal;
                                velA -= impulse * invMassA;
                                velB += impulse * invMassB;
                            }

                            // Positional correction
                            float penetration = minDist - dist;
                            glm::vec3 correction = normal * (penetration * 0.5f);
                            posA -= correction;
                            posB += correction;

                            // Write back
                            a.setPosition(posA);
                            b.setPosition(posB);
                            a.setVelocity(velA);
                            b.setVelocity(velB);
                        }
                    }
                }
            }
        }
    }
}



void PhysicsUpdater::applyBoundaries(float simDeltaTime, std::vector<Entity>& dynamicEntities)
{
    // Bouncing logic
    const float bounds = boxSize/2 - sphereRadius - wallThickness/2 ; // Sphere radius and wall thickness

    // First pass: apply gravity + wall bounces
    for (auto& entity : dynamicEntities) {
        glm::vec3 pos = entity.getPosition();
        glm::vec3 vel = entity.getVelocity();

        // Gravity
        vel += glm::vec3(0.0f, -9.81f, 0.0f) * simDeltaTime;
        pos += vel * simDeltaTime;

        // Bounce against boundaries
        if (pos.y < -bounds) { pos.y = -bounds; vel.y *= -1.0f; }
        if (pos.x < -bounds) { pos.x = -bounds; vel.x *= -1.0f; }
        if (pos.x >  bounds) { pos.x =  bounds; vel.x *= -1.0f; }
        if (pos.z < -bounds) { pos.z = -bounds; vel.z *= -1.0f; }
        if (pos.z >  bounds) { pos.z =  bounds; vel.z *= -1.0f; }
        if (pos.y >  bounds) { pos.y =  bounds; vel.y *= -1.0f; }
    
            // Write back

        entity.setPosition(pos);
        entity.setVelocity(vel);
    }
}


void PhysicsUpdater::SpatialHashGrid::clear(){
    gridCells.clear();
}

void PhysicsUpdater::SpatialHashGrid::insert(const glm::vec3& pos, size_t entityIndex)
{
    glm::ivec3 cellCoord = glm::floor(pos / cellSize);
    gridCells[cellCoord].push_back(entityIndex);
}

const std::vector<size_t> PhysicsUpdater::SpatialHashGrid::query(const glm::vec3& pos) // May want to make this a const return - come back to this
{

    glm::ivec3 cellCoord = glm::floor(pos / cellSize);
    auto it = gridCells.find(cellCoord);

    if (it != gridCells.end()) {
        return it->second; // Return the vector of indices if there are any
    } else {
        return {}; 
    }
}
