#include "RadioSettings.h"

#include "../util/PbUtils.h"

ResultCode
RadioSettings::readProtobuf(const uint8_t *buffer, size_t msg_length) {
  return PbUtils::readProtobuf<RadioSettings_RadioSettingsPb>(
    buffer,
    msg_length,
    RadioSettings_RadioSettingsPb_fields,
    RadioSettings_RadioSettingsPb_init_zero,
    m_rawSettings
  );
}
