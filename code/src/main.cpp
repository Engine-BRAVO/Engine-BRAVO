#include <iostream>
#include "terribleStopwatch.h"

int main()
{

  std::cout << "Start program" << std::endl;

  terribleStopwatch pocState;
  pocState.run();

  std::cout << "End program" << std::endl;

  return 0;
}