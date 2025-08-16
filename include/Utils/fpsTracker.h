#pragma once
#include <cstdint>
#include <iostream>

class FpsTracker {
public:
    FpsTracker();

    // Call once per frame with the current time in seconds (e.g. from glfwGetTime()).
    void update(float currentTime);

    // Returns the most recently calculated FPS.
    float getFps() const;

    float getAverageFps() const;  //average FPS


    // Optional: reset counters manually.
    void reset();

private:
    float fps;           // last calculated FPS
    float displayFps;    // FPS value to display
    uint32_t frameCount; // frames counted in the current interval
    float lastTime;      // last time FPS was calculated

    uint32_t totalFrames; // Total frames since start
    float startTime; // App start time
};