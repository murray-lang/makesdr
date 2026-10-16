#include "config/json/CapabilitiesConfig.json.h"
#include "config/json/TxCapabilitiesConfig.json.h"
#include "config/json/IqCapabilitiesConfig.json.h"
#include "config/json/FrequencyRangesConfig.json.h"

namespace Config::Capabilities
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    if (json["rx"].is<JsonVariantConst>()) {
      fields.rx = json["rx"].as<bool>();
    } else {
      fields.rx = true;
    }
    if (json["tx"].is<JsonVariantConst>()) {
      fields.tx.emplace<TxCapabilities::Fields>({});
      Config::TxCapabilities::fromJson(json["tx"], *fields.tx);
    }
    if (json["iq"].is<JsonVariantConst>()) {
      fields.iq.emplace<IqCapabilities::Fields>({});
      Config::IqCapabilities::fromJson(json["iq"], *fields.iq);
    }
    if (json["frequencyRanges"].is<JsonVariantConst>()) {
      Config::FrequencyRanges::fromJson(json["frequencyRanges"], fields.frequencyRanges);
    } else {
      return ResultCode::ERR_CONFIG_CAPABILITIES_MISSING_FREQUENCY_RANGE;
    }
    return ResultCode::OK;
  }
}
