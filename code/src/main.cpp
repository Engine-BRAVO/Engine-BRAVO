#include <iostream>
#include "terribleStopwatch.h"
#include "stopwatch.h"
#include "game.h"

int main()
{
  std::cout << "Start program" << std::endl;

  game game;
  game.run();

  // stopwatch pocStopwatch;
  // pocStopwatch.runStopwatch();

  // terribleStopwatch tsStopwatch;
  // tsStopwatch.run();

  std::cout << "End program" << std::endl;

  return 0;
}