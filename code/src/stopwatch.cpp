#include "stopwatch.h"

void stopwatch::runStopwatch()
{
    Stopwatch stopwatch;
    stopwatch.initiate();

    stopwatch.printElapsedTime();
    stopwatch.process_event(MakeActive());                // go to active state
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for 1 second
    stopwatch.printElapsedTime();

    stopwatch.process_event(StartStop());
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for 1 second
    stopwatch.printElapsedTime();

    stopwatch.process_event(StartStop());
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for 1 second
    stopwatch.printElapsedTime();

    stopwatch.process_event(Reset());
    stopwatch.printElapsedTime();
}