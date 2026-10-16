#include "config/json/RadioConfig.json.h"
#include "config/json/CapabilitiesConfig.json.h"
#include "config/json/ControlConfig.json.h"
#include "config/json/SdrConfig.json.h"
#include "config/json/UiConfig.json.h"



namespace Config::Radio
{
ResultCode fromJson(const JsonVariantConst& json, Fields& fields)
{
  ResultCode result = ResultCode::OK;
  if (json[Capabilities::type].is<JsonVariantConst>()) {
    result = Capabilities::fromJson(json[Capabilities::type], fields.capabilities);
  } else {
    return ResultCode::ERR_CONFIG_MISSING_CAPABILITIES;
  }
  if (result != ResultCode::OK) return result;

  if (json[Control::type].is<JsonVariantConst>()) {
    // TypedJson controlJson;
    // result = controlJson.fromJson(json[Control::type]);
    //
    // if (result == ResultCode::OK) {
      result = Control::fromJson(json[Control::type], fields.control);
    // }
  }
  if (result != ResultCode::OK) return result;

  if (json[Sdr::type].is<JsonVariantConst>()) {
    fields.sdr.emplace<Config::Sdr::Fields>({});
    result = Sdr::fromJson(json[Sdr::type], *fields.sdr);
  }
  if (result != ResultCode::OK) return result;

  if (json[Ui::type].is<JsonVariantConst>()) {
    fields.ui.emplace<Config::Ui::Fields>({});
    result = Ui::fromJson(json[Ui::type], *fields.ui);
  }
  return result;
}
}

// inline Config::Radio::Fields tempConfig {
//   .control{
//     .sinks = {
//       Config::Control::SinkConfigVariant {
//         Config::DigitalOutput::Fields {
//           "digitaloutput",
//           Config::GpioLines::Fields{
//             .lines{ 23 },
//             .direction = "output", // Shouldn't have to set this for outputs
//             .bias = "none",
//             .edge = "rising" // What about level?
//           },
//           "ptt" // settingPath
//         }
//       },
//       Config::Control::SinkConfigVariant{
//         Config::FunCube::Fields{ "funcube" }
//       }
//     },
//     // .sources = {
//     // }
//   },
//   .receiver{
//     .iqIo{
//     }
//   },
//   .transmitter{
//     .iqIo{
//     }
//   },
//   .ui{
//     .face = "default"
//   }
// };