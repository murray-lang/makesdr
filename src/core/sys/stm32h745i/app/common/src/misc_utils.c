#include "sys/stm32h745i/app/common/misc_utils.h"



uint8_t compareBufs(const uint8_t* pBuffer1, const uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}
