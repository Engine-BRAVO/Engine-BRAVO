#include "poc_state.hpp"

#include <iostream>

void PocState::run()
{
    StopWatch myWatch;
    myWatch.initiate();
    std::cout << myWatch.ElapsedTime() << "\n";
    myWatch.process_event(EvStartStop());
    std::cout << myWatch.ElapsedTime() << "\n";
    myWatch.process_event(EvStartStop());
    std::cout << myWatch.ElapsedTime() << "\n";
    myWatch.process_event(EvStartStop());
    std::cout << myWatch.ElapsedTime() << "\n";
    myWatch.process_event(EvReset());
    std::cout << myWatch.ElapsedTime() << "\n";
}