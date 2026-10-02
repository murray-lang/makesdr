#pragma once

#include <CrossPlatformTypes.h>
#include <settings/model/message/PayloadType.h>
#include <ResultCode.h>
#include <settings/model/radio/basic/BasicRxSettings.h>
#include <settings/model/radio/basic/BasicRxTxSettings.h>
#include <settings/model/radio/iq/BasicIqRxSettings.h>
#include <settings/model/radio/iq/BasicIqRxTxSettings.h>
#include <settings/model/radio/iq/DualIqRxSettings.h>
#include <settings/model/radio/iq/DualIqRxTxSettings.h>
#include <settings/model/radio/iq/SplitBandDualIqRxTxSettings.h>

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


