#pragma once
#include <thread/Semaphore.h>

struct SemaphoreNotifier { Semaphore& sem;   void operator()() { sem.post(); } };
