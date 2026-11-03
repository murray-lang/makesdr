#ifndef STM32H745_AUDIO_OUTPUT_FACTORY_H
#define STM32H745_AUDIO_OUTPUT_FACTORY_H

// #include "AudioOutput.h"
#include "AudioOutputVariant.h"
#include <config/struct/IqIoConfig.h>

class AudioOutputFactory
{
public:
  static ResultCode create(const Config::IqIo::AudioOutputConfigVariant& config, AudioOutputVariant& output);
};

#endif // STM32H745_AUDIO_OUTPUT_FACTORY_H