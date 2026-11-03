#include "transport/stm32h745i/HsemNotifier.h"

#include <stm32h745i/drivers/hal/stm32h7xx_hal.h>
#include <transport/PayloadTypeBits.h>

namespace
{
  // Which task to wake, and with which bit, for each semaphore whose ring is
  // consumed on this core. Each core's image has its own copy.
  struct Channel
  {
    TaskHandle_t task;
    uint32_t     bits;
  };

  Channel  s_channels[HsemNotifier::LastId + 1] = {};
  uint32_t s_attachedMask = 0;

  // Used only on the core that constructs the rings.
  uint32_t s_nextId = HsemNotifier::FirstId;

  constexpr uint32_t maskOf(uint32_t semId) { return uint32_t{1} << semId; }

  // Each core has its own HSEM interrupt line.
#if defined(CORE_CM7)
  constexpr IRQn_Type HsemIrq = HSEM1_IRQn;
#elif defined(CORE_CM4)
  constexpr IRQn_Type HsemIrq = HSEM2_IRQn;
#else
#error "Define CORE_CM7 or CORE_CM4"
#endif
}

//--- interrupt plumbing ------------------------------------------------------
// Defined here rather than in a setup, so that only programs using shared
// rings take over the HSEM interrupt; these replace the startup file's weak
// defaults. A program needing HAL_HSEM_FreeCallback for something else will
// get a duplicate-definition error, and should call onSemaphoresFreed() from
// its own.

extern "C" void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  HsemNotifier::onSemaphoresFreed(SemMask);
}

#if defined(CORE_CM7)
extern "C" void HSEM1_IRQHandler(void) { HAL_HSEM_IRQHandler(); }
#else
extern "C" void HSEM2_IRQHandler(void) { HAL_HSEM_IRQHandler(); }
#endif

//-----------------------------------------------------------------------------

HsemNotifier::HsemNotifier()
  : m_semId(static_cast<uint8_t>(s_nextId++))
{
  configASSERT(m_semId <= LastId);  // more shared rings than free semaphores
}

void HsemNotifier::attachTask(TaskHandle_t task, PayloadType payloadType)
{
  configASSERT(task != nullptr);    // TaskDispatcher not bound before attaching
  const uint32_t mask = maskOf(m_semId);

  // The interrupt reads these, and the HAL's enable is a read-modify-write of
  // a register its handler also writes.
  taskENTER_CRITICAL();
  s_channels[m_semId] = Channel{task, PayloadTypeBits::maskOf(payloadType)};
  s_attachedMask |= mask;
  HAL_HSEM_ActivateNotification(mask);
  taskEXIT_CRITICAL();

  // Highest priority allowed to call FreeRTOS FromISR functions. Repeating
  // this for each ring is harmless.
  HAL_NVIC_SetPriority(HsemIrq, MAKESDR_HSEM_IRQ_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(HsemIrq);
}

void HsemNotifier::detach()
{
  const uint32_t mask = maskOf(m_semId);

  taskENTER_CRITICAL();
  HAL_HSEM_DeactivateNotification(mask);
  s_attachedMask &= ~mask;
  s_channels[m_semId] = Channel{};
  taskEXIT_CRITICAL();
}

void HsemNotifier::operator()(PayloadType)
{
  // The slot and the ring's index must be visible in SRAM4 before the other
  // core can be interrupted to read them.
  __DSB();

  // Taking and releasing raises the consumer core's interrupt. If the take
  // fails, the semaphore is held by the consumer itself (waking itself on
  // attach), and its release will raise the interrupt instead.
  if (HAL_HSEM_FastTake(m_semId) == HAL_OK) {
    HAL_HSEM_Release(m_semId, 0);
  }
}

void HsemNotifier::onSemaphoresFreed(uint32_t semMask)
{
  const uint32_t mine = semMask & s_attachedMask;
  if (mine == 0) {
    return;
  }

  // HAL_HSEM_IRQHandler() disabled these before calling back. Re-enable them
  // before waking anyone, so that a release while the task drains raises a
  // fresh interrupt rather than being missed.
  HAL_HSEM_ActivateNotification(mine);

  BaseType_t higherPriorityTaskWoken = pdFALSE;
  uint32_t pending = mine;
  while (pending != 0) {
    const uint32_t semId = static_cast<uint32_t>(__builtin_ctz(pending));
    pending &= pending - 1;
    const Channel& channel = s_channels[semId];
    if (channel.task != nullptr) {
      TaskNotify::setBitsFromISR(channel.task, channel.bits, &higherPriorityTaskWoken);
    }
  }
  portYIELD_FROM_ISR(higherPriorityTaskWoken);
}
