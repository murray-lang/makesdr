#ifndef STM32H745_AUDIO_OUTPUT_VARIANT_H_
#define STM32H745_AUDIO_OUTPUT_VARIANT_H_

#include <etl/variant.h>
#include "SaiAudio.h"

using AudioOutputVariant = etl::variant<etl::monostate, SaiAudio>;

#endif // STM32H745_AUDIO_OUTPUT_VARIANT_H_
