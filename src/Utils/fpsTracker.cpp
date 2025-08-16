#include "Utils/fpsTracker.h"

FpsTracker::FpsTracker()
    : fps(0.0f), frameCount(0), lastTime(0.0f), totalFrames(0)
{
}

void FpsTracker::update(float currentTime) {
    frameCount++;
    totalFrames++;

    float delta = currentTime - lastTime;
    if (delta >= 0.5f) {
        fps = static_cast<float>(frameCount) / delta;
        frameCount = 0;
        lastTime = currentTime;
        std::cout<< "FPS: " << fps << std::endl;
        displayFps = fps;
    }

    // Initialise startTime on the first frame
    if (totalFrames == 1) {
        startTime = currentTime;
    }
}

float FpsTracker::getFps() const {
    return displayFps;
}

float FpsTracker::getAverageFps() const {

    float totalTime = lastTime - startTime; // Total time since start    
    return static_cast<float>(totalFrames) / totalTime;
}

void FpsTracker::reset() {
    fps = 0.0f;
    frameCount = 0;
    lastTime = 0.0f;
    totalFrames = 0;
    startTime = 0.0f;
    displayFps = 0.0f;
}
