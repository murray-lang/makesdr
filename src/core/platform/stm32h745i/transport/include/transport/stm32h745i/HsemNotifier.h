#pragma once

#include <cstdint>
#include <type_traits>

#include <settings/model/message/PayloadType.h>
#include <transport/freertos/TaskNotify.h>

#ifndef MAKESDR_HSEM_RING_FIRST_ID
// Hardware semaphores below this are left for other uses: 0 wakes the CM4 at
// boot, 2 guards the shared UART, 3 is reserved for the shared I2C4 bus.
#define MAKESDR_HSEM_RING_FIRST_ID 8
#endif

#ifndef MAKESDR_HSEM_IRQ_PRIORITY
// Must be numerically >= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, so the
// interrupt may wake tasks.
#define MAKESDR_HSEM_IRQ_PRIORITY configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
#endif

//*****************************************************************************
// Wakes the consumer of a ring shared between the STM32H745's two cores, by
// releasing a hardware semaphore (HSEM) whose "freed" interrupt is enabled
// only on the consumer's core. There the interrupt sets the ring's payload
// type bit in the consumer task's transport notification value, exactly as
// TaskNotifier would, so TaskDispatcher and TaskRecipientT serve both.
//
// Rings hold only plain payload structs, and the notifier itself holds
// nothing but its semaphore number, so the whole ring can live in shared
// memory (SRAM4) and be used from both images.
//
// Semaphore numbers are handed out in construction order, starting at
// MAKESDR_HSEM_RING_FIRST_ID, by the core that constructs the rings. Only one
// core may construct them (see MessageRingBufferT); the other reads each
// number from shared memory. Everything else that varies at run time (which
// task to wake, which bit) is kept in tables local to each core.
//
// The interrupt plumbing is in HsemNotifier.cpp: HSEM1_IRQHandler (CM7) or
// HSEM2_IRQHandler (CM4) and HAL_HSEM_FreeCallback() are defined there, and
// the core's HSEM interrupt is enabled when its first ring attaches, at
// MAKESDR_HSEM_IRQ_PRIORITY. The application only needs the HSEM clock on
// (__HAL_RCC_HSEM_CLK_ENABLE(), already done at boot on both cores).
//
// attach() and detach() are called from a task on the consumer's core (or
// before its scheduler starts); detach() is then a hard barrier, since the
// interrupt it could race with runs on the same core and is masked meanwhile.
//*****************************************************************************
class HsemNotifier
{
public:
  static constexpr uint32_t FirstId = MAKESDR_HSEM_RING_FIRST_ID;
  static constexpr uint32_t LastId = 31;

  // Takes the next free semaphore number. Constructing core only.
  HsemNotifier();

  //--- consumer core ---------------------------------------------------------

  template<typename TargetT>
  void attach(TargetT* target, PayloadType payloadType)
  {
    attachTask(target->taskHandle(), payloadType);
  }

  void detach();

  //--- producer core (task or ISR) -------------------------------------------

  void operator()(PayloadType payloadType);

  //---------------------------------------------------------------------------

  [[nodiscard]] uint32_t semaphoreId() const { return m_semId; }

  // From HAL_HSEM_FreeCallback(SemMask), on each core that consumes. Ignores
  // semaphores that are not attached rings on this core.
  static void onSemaphoresFreed(uint32_t semMask);

private:
  void attachTask(TaskHandle_t task, PayloadType payloadType);

  uint8_t m_semId;
};

static_assert(!std::is_polymorphic_v<HsemNotifier>);
static_assert(std::is_trivially_copyable_v<HsemNotifier>);
