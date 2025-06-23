#pragma once

#include <chrono>

class Timer {
public:
    Timer();

    void reset();                     // Reset the timer to current time
    void update();                    // Update current time and compute delta

    float getDeltaTime() const;       // Time between frames (in seconds)
    float getTimeSinceStart() const;  // Total time since start (in seconds)

private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint startTime;
    TimePoint lastTime;
    float deltaTime = 0.0f; // Time between frames in seconds
};
