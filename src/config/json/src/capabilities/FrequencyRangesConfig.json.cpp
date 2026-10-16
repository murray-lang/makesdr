#include "config/json/FrequencyRangesConfig.json.h"
#include "config/json/FrequencyRangeConfig.json.h"

namespace Config::FrequencyRanges
{
  ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
  {
    fields.frequencyRanges.clear();

    ResultCode result = ResultCode::OK;
    // if (json["frequencyRanges"]) {
      for (JsonVariantConst rangeJson : json.as<JsonArrayConst>()) {
        FrequencyRange::Fields frequencyRangeFields{};
        result = FrequencyRange::fromJson(rangeJson, frequencyRangeFields);

        if (result != ResultCode::OK) return result;

        fields.frequencyRanges.emplace_back(frequencyRangeFields);
      }
    // }
    return ResultCode::OK;
  }
}
