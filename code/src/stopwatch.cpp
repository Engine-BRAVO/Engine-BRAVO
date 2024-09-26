#include "stopwatch.h"

void stopwatch::runStopwatch()
{
    Stopwatch stopwatch;
    stopwatch.initiate();

    // Simulate events
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stopwatch.process_event(MakeActive()); // go to active state
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stopwatch.process_event(StartStop()); // go to running state
    std::this_thread::sleep_for(std::chrono::seconds(1));
    stopwatch.process_event(StartStop()); // go to stop state
    stopwatch.process_event(Reset());
}