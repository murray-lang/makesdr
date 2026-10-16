#pragma once
#include <variant>
#include <radios/settings/model/DualIqRxSettings.h>
#include <radios/settings/model/DualIqRxTxSettings.h>
#include <radios/settings/model/SplitBandDualIqRxTxSettings.h>

using RadioSettingsVariant = std::variant<
  DualIqRxSettings,
  DualIqRxTxSettings,
  SplitBandDualIqRxTxSettings
>;
