#include "SimInfo/fpsTracker.h"

FpsTracker::FpsTracker()
    : fps(0.0f), frameCount(0), lastTime(0.0f)
{
}

void FpsTracker::update(float currentTime) {
    frameCount++;

    float delta = currentTime - lastTime;
    if (delta >= 1.0f) {
        fps = static_cast<float>(frameCount) / delta;
        frameCount = 0;
        lastTime = currentTime;
        std::cout<< "FPS: " << fps << std::endl;
    }
}

float FpsTracker::getFps() const {
    return fps;
}

void FpsTracker::reset() {
    fps = 0.0f;
    frameCount = 0;
    lastTime = 0.0f;
}
