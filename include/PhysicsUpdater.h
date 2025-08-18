#pragma once

#include <vector>

#include "Utils/fpsTracker.h"
#include "Entity.h"

class PhysicsUpdater
{
    
public:
    PhysicsUpdater(float boxSize, float sphereRadius, float wallThickness);

    void update(float deltaTimeMultiplier, float deltaTime, std::vector<Entity>& dynamicEntities);

private:

    float boxSize;
    float sphereRadius; 
    float wallThickness;

};