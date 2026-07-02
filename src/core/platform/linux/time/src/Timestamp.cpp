#include "time/Timestamp.h"

#include <ctime>


Timestamp now()
{
  timespec now{};
  clock_gettime(CLOCK_MONOTONIC, &now);
  return static_cast<Timestamp>(now.tv_sec) * 1'000'000'000ULL + now.tv_nsec;
}
