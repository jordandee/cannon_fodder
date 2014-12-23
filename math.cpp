#include <iostream>
#include <cstdlib>
#include <ctime>
#include "math.h"

using std::rand;
using std::srand;
using std::time;

// return random int between [0,n)
int nrand(int n)
{
  if (n <= 0 || n > RAND_MAX)
    std::cout << "Argument to nrand is out of range" << std::endl;

  static bool been_called = false;
  if (!been_called)
  {
    srand(time(NULL));
    been_called = true;
  }

  int r;
  const int bucket_size = RAND_MAX / n;

  do r = rand() / bucket_size;
  while (r >= n);

  return r;
}
