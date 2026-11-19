#include "stm32h745i_discovery_audio.h"
#include "audio.h"

#define AUDIO_BUF_SIZE  4096  /* Size in bytes (e.g., 1024 stereo 16-bit samples) */

uint8_t audio_buffer[AUDIO_BUF_SIZE];
volatile uint8_t half_transfer_flag = 0;
volatile uint8_t full_transfer_flag = 0;

BSP_AUDIO_Init_t audioConfig;

void startMicrophone()
{
  audioConfig.Device        = AUDIO_IN_DEVICE_ANALOG_MIC; /* 3.5mm Headset/Line-In jack */
  audioConfig.ChannelsNbr   = 2;
  audioConfig.SampleRate    = AUDIO_FREQUENCY_48K;
  audioConfig.BitsPerSample = AUDIO_RESOLUTION_16B;
  audioConfig.Volume        = 80;

  if (BSP_AUDIO_IN_Init(0, &audioConfig) == BSP_ERROR_NONE)
  {
    BSP_AUDIO_IN_Record(0, audio_buffer, AUDIO_BUF_SIZE);
  }
}

void stopMicrophone()
{
  BSP_AUDIO_IN_Stop(0);
  BSP_AUDIO_IN_DeInit(0);
}

void startSpeaker()
{
  audioConfig.Device        = AUDIO_OUT_DEVICE_HEADPHONE; /* 3.5mm Headphone/Line-Out jack */
  audioConfig.ChannelsNbr   = 2;
  audioConfig.SampleRate    = AUDIO_FREQUENCY_48K;
  audioConfig.BitsPerSample = AUDIO_RESOLUTION_16B;
  audioConfig.Volume        = 70;

  if (BSP_AUDIO_OUT_Init(0, &audioConfig) == BSP_ERROR_NONE)
  {
    BSP_AUDIO_OUT_Play(0, audio_buffer, AUDIO_BUF_SIZE);
  }
}

void stopSpeaker()
{
  BSP_AUDIO_OUT_Stop(0);
  BSP_AUDIO_OUT_DeInit(0);
}


// /* BSP Audio Out Callbacks */
// void BSP_AUDIO_OUT_HalfTransfer_CallBack(uint32_t Instance)
// {
//   half_transfer_flag = 1; /* Safe to refill first half of audio_buffer */
// }
//
// void BSP_AUDIO_OUT_TransferComplete_CallBack(uint32_t Instance)
// {
//   full_transfer_flag = 1; /* Safe to refill second half of audio_buffer */
// }
//
// /* BSP Audio In Callbacks */
// void BSP_AUDIO_IN_HalfTransfer_CallBack(uint32_t Instance)
// {
//   half_transfer_flag = 1; /* First half of audio_buffer has fresh mic data */
// }
//
// void BSP_AUDIO_IN_TransferComplete_CallBack(uint32_t Instance)
// {
//   full_transfer_flag = 1; /* Second half of audio_buffer has fresh mic data */
// }
