#include <chrono>
#include "timer.h"

void Timer::start()
{
  t = std::chrono::high_resolution_clock::now();
}

double Timer::getTime()
{
  std::chrono::high_resolution_clock::time_point tnow;
  tnow = std::chrono::high_resolution_clock::now();

  return std::chrono::duration_cast<std::chrono::duration<double>>(tnow-t).count();
}
