/* USER CODE BEGIN Includes */
#include <climits>

#include "stm32h745i_discovery_audio.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

#include "stm32h745i_discovery.h"
#include "stm32h745i/app/support/safe_printf.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Variables */
#define LOOPBACK_BUF_SIZE   128000U   /* 64,000 16-bit half-words (< 65,535 DMA limit) */
#define AUDIO_DONE_BIT      (1UL << 0)

#define HALF_BUF_BIT  (1UL << 0)   /* Bit 0 (0x00000001): First half of buffer ready  */
#define FULL_BUF_BIT  (1UL << 1)   /* Bit 1 (0x00000002): Second half of buffer ready */


#define RX_FRAMES      512U                 /* 1 slot/frame  -> 1024 bytes */
#define TX_SLOTS       4U                   /* SAI2_Block_A: slots 0..3    */

static int16_t rx_buffer[RX_FRAMES];
static int16_t tx_buffer[RX_FRAMES * TX_SLOTS];


static TaskHandle_t xAudioTaskHandle = NULL;
static BSP_AUDIO_Init_t audioConfig;
/* USER CODE END Variables */

static void expand(const int16_t *in, int16_t *out, uint32_t frames)
{
  for (uint32_t i = 0; i < frames; ++i) {
    const int16_t s = in[i];
    out[0] = s; out[1] = s; out[2] = s; out[3] = s;
    out += TX_SLOTS;
  }
}

static volatile uint32_t s_inHalf, s_inFull, s_outHalf, s_outFull, s_inErr, s_outErr;

extern SAI_HandleTypeDef haudio_in_sai;
extern SAI_HandleTypeDef haudio_out_sai;

/* =========================================================================
   Native FreeRTOS Audio Loopback Task
   ========================================================================= */
void vAudioLoopbackTask(void *pvParameters)
{

  uint32_t ulNotifiedValue = 0;
  xAudioTaskHandle = xTaskGetCurrentTaskHandle();

  /* 1. INITIALISE ONCE BEFORE THE LOOP */
  audioConfig.ChannelsNbr   = 2;
  audioConfig.SampleRate    = AUDIO_FREQUENCY_48K;
  audioConfig.BitsPerSample = AUDIO_RESOLUTION_16B;

  /* Start Output first so SAI2_Block_A generates SCK and FS clocks */
  audioConfig.Device = AUDIO_OUT_DEVICE_HEADPHONE;
  audioConfig.Volume = 70;
  int32_t rc = BSP_AUDIO_OUT_Init(0, &audioConfig);
  if (rc != BSP_ERROR_NONE) {
    SAFE_PRINTF("BSP_AUDIO_OUT_Init failed with rc=%ld\r\n", rc);
  } else {
    SAFE_PRINTF("BSP_AUDIO_OUT_Init succeeded\r\n");
  }

  /* Start Input on SAI2_Block_B (slaves to SAI2_Block_A's clocks) */
  audioConfig.Device = AUDIO_IN_DEVICE_ANALOG_MIC;
  audioConfig.Volume = 75;
  rc = BSP_AUDIO_IN_Init(0, &audioConfig);
  if (rc != BSP_ERROR_NONE) {
    SAFE_PRINTF("BSP_AUDIO_IN_Init failed with rc=%ld\r\n", rc);
  } else {
    SAFE_PRINTF("BSP_AUDIO_IN_Init succeeded\r\n");
  }

  /* Kick off continuous circular DMA for both buffers (e.g. 1024 bytes each) */
  rc = BSP_AUDIO_IN_Record(0, reinterpret_cast<uint8_t*>(rx_buffer), sizeof rx_buffer);
  if (rc != BSP_ERROR_NONE) {
    SAFE_PRINTF("BSP_AUDIO_IN_Record failed with rc=%ld\r\n", rc);
  } else {
    SAFE_PRINTF("BSP_AUDIO_IN_Record succeeded\r\n");
  }

  rc = BSP_AUDIO_OUT_Play(0, reinterpret_cast<uint8_t*>(tx_buffer), sizeof tx_buffer);
  if (rc != BSP_ERROR_NONE) {
    SAFE_PRINTF("BSP_AUDIO_OUT_Play failed with rc=%ld\r\n", rc);
  } else {
    SAFE_PRINTF("BSP_AUDIO_OUT_Play succeeded\r\n");
  }


  TickType_t lastPrint = xTaskGetTickCount();
  /* 2. REAL-TIME STREAMING LOOP (NO INIT/DEINIT EVER) */
  for (;;)
  {
    // xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
    ulNotifiedValue = 0;
    xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, pdMS_TO_TICKS(1000));

    if (ulNotifiedValue & HALF_BUF_BIT) //TX finished first half: refill it
    {
      expand(rx_buffer, tx_buffer, RX_FRAMES / 2);
    }

    if (ulNotifiedValue & FULL_BUF_BIT) // TX finished second half: refill it
    {
      expand(&rx_buffer[RX_FRAMES / 2], &tx_buffer[(RX_FRAMES / 2) * TX_SLOTS], RX_FRAMES / 2);
    }
  }
}


/* =========================================================================
   BSP DMA Interrupt Callbacks (Native FreeRTOS FromISR Notification)
   ========================================================================= */
extern "C" void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
{
  BaseType_t woken = pdFALSE;
  if ((Instance == 0) && (xAudioTaskHandle != NULL))
  {
    xTaskNotifyFromISR(xAudioTaskHandle, HALF_BUF_BIT, eSetBits, &woken);  // first half of rx_buffer ready
    portYIELD_FROM_ISR(woken);
  }
}

extern "C" void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
{
  BaseType_t woken = pdFALSE;
  if ((Instance == 0) && (xAudioTaskHandle != NULL))
  {
    xTaskNotifyFromISR(xAudioTaskHandle, FULL_BUF_BIT, eSetBits, &woken);  // second half of rx_buffer ready
    portYIELD_FROM_ISR(woken);
  }
}
