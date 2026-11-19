#include <errno.h>
#include <stddef.h>

#include "FreeRTOS.h"

// Stand-in for posix_memalign(), which newlib on arm-none-eabi doesn't provide.
// liquid-dsp only uses it via liquid_aligned_alloc(), whose memory is released
// with free() -> vPortFree(), so we must hand back the raw pvPortMalloc()
// pointer and cannot over-allocate and offset it. That means the requested
// alignment is not honoured beyond what the FreeRTOS heap gives (heap_3 wraps
// newlib malloc: 8 bytes). liquid asks for 64 as a SIMD/cache-line hint; the
// portable dotprod used on Cortex-M only needs natural alignment, so this is OK.
int liquid_freertos_posix_memalign(void** memptr, size_t alignment, size_t size)
{
  if (alignment == 0 || (alignment & (alignment - 1)) != 0 || (alignment % sizeof(void*)) != 0) {
    return EINVAL;
  }
  void* ptr = pvPortMalloc(size);
  if (ptr == NULL) {
    return ENOMEM;
  }
  *memptr = ptr;
  return 0;
}
