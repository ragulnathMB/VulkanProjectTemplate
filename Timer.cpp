#include "Timer.hpp"

Timer::Timer() {
    reset();
}

void Timer::reset() {
    startTime = Clock::now();
    lastTime = startTime;
    deltaTime = 0.0f;
}

void Timer::update() {
    TimePoint currentTime = Clock::now();
    deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
}

float Timer::getDeltaTime() const {
    return deltaTime;
}

float Timer::getTimeSinceStart() const {
    return std::chrono::duration<float>(Clock::now() - startTime).count();
}
