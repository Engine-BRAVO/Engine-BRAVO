#include "Time.h"

// Initialize static members
std::chrono::high_resolution_clock::time_point Time::startTime;
std::chrono::high_resolution_clock::time_point Time::lastFrameTime;
double Time::deltaTime = 0.0;
double Time::ticks = 0.0;
