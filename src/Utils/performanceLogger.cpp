#include "Utils/performanceLogger.h"
#include <iostream>

void PerformanceLogger::beginStage(const std::string& name) {
    startPoints[name] = Clock::now(); // Will create if the name doesn't exist already
}

void PerformanceLogger::endStage(const std::string& name) {
    auto now = Clock::now();
    auto it = startPoints.find(name);
    if (it != startPoints.end()) {
        totals[name] += std::chrono::duration<double, std::milli>(now - it->second).count(); // Makes sense eventually!
        startPoints.erase(it);
    }
}

void PerformanceLogger::report(float averageFPS) const {

    // Messy way of doing this but isn't critical so cba
    double totalTimeLogged = 0.0f;  
    for (const auto& [stage, time] : totals) {
        totalTimeLogged += time;
    }
    
    std::cout << "\n------Perfomance Report------\n";
    for (const auto& [stage, time] : totals) {
        float percentage = (time / totalTimeLogged) * 100;
        std::cout << stage << ": " << time << " ms total (" << percentage << "% of logged time)\n";
    }
    std::cout << "\nAverage FPS: " << averageFPS << "\n";
    std::cout << "-----------------------------\n\n";
}
