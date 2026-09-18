#pragma once

#include <CrossPlatformTypes.h>
#include <settings/model/message/PayloadType.h>
#include <ResultCode.h>
#include <settings/model/radios/basic/BasicRxSettings.h>
#include <settings/model/radios/basic/BasicRxTxSettings.h>
#include <settings/model/radios/iq/BasicIqRxSettings.h>
#include <settings/model/radios/iq/BasicIqRxTxSettings.h>
#include <settings/model/radios/iq/DualIqRxSettings.h>
#include <settings/model/radios/iq/DualIqRxTxSettings.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

using RadioSettingsVariant = variant<
  // BasicRxSettings,
  // BasicRxTxSettings,
  // BasicIqRxSettings,
  // BasicIqRxTxSettings,
  // DualIqRxSettings,
  // DualIqRxTxSettings,
  SplitBandDualIqRxTxSettings
>;

extern ResultCode emplaceVariant(PayloadType payloadType, RadioSettingsVariant& variant);


