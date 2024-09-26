#include <iostream>
#include "terribleStopwatch.h"
#include "stopwatch.h"

int main()
{

  std::cout << "Start program" << std::endl;

  stopwatch pocStopwatch;
  pocStopwatch.runStopwatch();

  std::cout << "End program" << std::endl;

  return 0;
}