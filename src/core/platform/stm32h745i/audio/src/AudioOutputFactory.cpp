#include "audio/AudioOutputFactory.h"

ResultCode
AudioOutputFactory::create(const Config::IqIo::AudioOutputConfigVariant& configVariant, AudioOutputVariant& output)
{
  if (holds_alternative<Config::SaiAudio::Fields>(configVariant)) {
    output.emplace<SaiAudio>();
    const ResultCode rc = get<SaiAudio>(output).configure(get<Config::SaiAudio::Fields>(configVariant));
    if (rc != ResultCode::OK) {
      output.emplace<monostate>();
    }
    return rc;
  }
  return ResultCode::ERR_AUDIO_OUTPUT_UNKNOWN_TYPE;
}
