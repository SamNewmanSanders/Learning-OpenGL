#pragma once
#include <chrono>
#include <unordered_map>
#include <string>

using Clock = std::chrono::high_resolution_clock; // Do this to avoid that long ass name

class PerformanceLogger {
public:

    PerformanceLogger() = default; // As std::unordered_map initializes automatically as empty
    ~PerformanceLogger() = default;

    void beginStage(const std::string& name);

    void endStage(const std::string& name);

    // At the end of the program
    void report(float averageFPS) const;

private:
    std::unordered_map<std::string, Clock::time_point> startPoints;
    std::unordered_map<std::string, double> totals;
};
