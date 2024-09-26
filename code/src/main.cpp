#include <iostream>
#include "poc_state.hpp"

int main()
{

  std::cout << "Start program" << std::endl;

  PocState pocState;
  pocState.run();

  std::cout << "End program" << std::endl;

  return 0;
}