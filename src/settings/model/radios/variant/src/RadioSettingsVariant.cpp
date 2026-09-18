#include "settings/model/radios/variant/RadioSettingsVariant.h"


extern ResultCode emplaceVariant(PayloadType payloadType, RadioSettingsVariant& var)
{
  switch (payloadType) {
  // case PayloadType::PAYLOAD_TYPE_SETTINGS_BASIC_RX:
  // case PayloadType::PAYLOAD_TYPE_SETTINGS_BASIC_RXTX:
  // case PayloadType::PAYLOAD_TYPE_SETTINGS_BASIC_IQ_RX:
  // case PayloadType::PAYLOAD_TYPE_SETTINGS_BASIC_IQ_RXTX:
  // case PayloadType::PAYLOAD_TYPE_SETTINGS_DUAL_IQ_RX:
  // case PayloadType::PAYLOAD_TYPE_SETTINGS_DUAL_IQ_RXTX:
  case PayloadType::PAYLOAD_TYPE_SETTINGS_SPLIT_BAND_DUAL_IQ_RXTX:
    var.emplace<SplitBandDualIqRxTxSettings>();
  }
  return ResultCode::ERR_PAYLOAD_NOT_SETTING_RELATED;
}