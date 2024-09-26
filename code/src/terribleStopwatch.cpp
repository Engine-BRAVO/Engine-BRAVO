#include "terribleStopwatch.h"

#include <iostream>
#include <thread> // Include the thread library
#include <chrono> // Include the chrono library

void terribleStopwatch::run()
{

    StopWatch myWatch;
    myWatch.initiate();
    std::cout << myWatch.ElapsedTime() << "\n";
    myWatch.process_event(EvStartStop());
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for 1 second
    std::cout << myWatch.ElapsedTime() << "\n";

    myWatch.process_event(EvStartStop());
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for 1 second
    std::cout << myWatch.ElapsedTime() << "\n";

    myWatch.process_event(EvStartStop());
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for 1 second
    std::cout << myWatch.ElapsedTime() << "\n";

    myWatch.process_event(EvReset());
    std::cout << myWatch.ElapsedTime() << "\n";
}