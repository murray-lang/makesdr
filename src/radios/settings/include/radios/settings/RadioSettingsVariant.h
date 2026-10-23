#pragma once
#include <variant>
#include <settings/model/radio/iq/DualIqRxSettings.h>
#include <settings/model/radio/iq/DualIqRxTxSettings.h>
#include <settings/model/radio/iq/SplitBandDualIqRxTxSettings.h>

using RadioSettingsVariant = std::variant<
  DualIqRxSettings,
  DualIqRxTxSettings,
  SplitBandDualIqRxTxSettings
>;
