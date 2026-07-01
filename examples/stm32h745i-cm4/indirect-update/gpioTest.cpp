#include "gpioTest.h"
#include <etl/vector.h>
#include "digitalInputConfigs.h"
#include <gpio/service/Gpio.h>
#include <stm32h745i/app/support/safe_printf.h>
#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>

#include "settings/control/digital/DigitalInputs.h"

#include "settings/model/proto/RadioSettings.pb.h"
#include "settings/model/meta/generalCoverageRadioMeta.h"
#include "../../data/exampleRadioSettings.h"

// class DigitalInputParent : public GpioInputLines
// {
// public:
//   DigitalInputParent()
//     : m_lineEventCallback(
//       GpioLineEventCallback::create<DigitalInputParent, &DigitalInputParent::handleGpioLineEvent>(*this)
//       )
//     , m_linesRequest(*this, m_lineEventCallback)
//   {
//   }
//   DigitalInputParent(const DigitalInputParent& rhs)
//     : DigitalInputParent()
//   {
//   }
//
//   GpioInputLinesRequest& getLinesRequest() { return m_linesRequest; }
//
//   ResultCode configure(const Config::DigitalInput::Fields& config)
//   {
//     return configureLines(config);
//   }
//
//   void handleGpioLineEvent(GpioLineEvent* event)
//   {
//     if (event->value > 0) {
//       BSP_LED_On(LED_GREEN);
//       BSP_LED_Off(LED_RED);
//     } else if (event->value < 0) {
//       BSP_LED_On(LED_RED);
//       BSP_LED_Off(LED_GREEN);
//     } else {
//       BSP_LED_Off(LED_RED);
//       BSP_LED_Off(LED_GREEN);
//     }
//   }
// protected:
//   GpioLineEventCallback m_lineEventCallback;
//   GpioInputLinesRequest m_linesRequest;
//
// };
//
// GpioInputLinesSource linesSource;
//
// DigitalInputParent digitalInput;
// DigitalInputParent rotaryEncoder;
//
// etl::vector<DigitalInputParent, 2> digitalInputs; //{ digitalInputVariant, rotaryEncoderVariant};

using RadioSettingsPb = makesdr_RadioSettingsPb;
RadioSettingsPb radioSettingsPb = makesdr_RadioSettingsPb_init_zero;
BandSettingsCache bandSettingsCache;

RadioSettings radioSettings(exampleRadioSettingsPb, generalCoverageRadioMeta, bandSettingsCache);

Gpio& gpio = Gpio::getInstance();

DigitalInputs digitalInputs;

ResultCode gpioTest()
{


  ResultCode rc = digitalInputs.configure(digitalInputsConfig);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t DigitalInputs::configure() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }

  const DigitalInputVector& inputs = digitalInputs.getInputs();
  const SettingPath& path = inputs.at(1).getSettingPath();
  // printf("[CM4]\tgpioTest() SettingPath: %lu, %lu, %lu, %lu, %lu, %lu\r\n", path[0], path[1], path[2], path[3], path[4], path[5]);
  // const DigitalInputVector& inputs = digitalInputs.getInputs();
  // for (auto& input : inputs) {
  //   SAFE_PRINTF("[CM4]\t Lines: %0x\r\n", input.getLines());
  // }


  // if (!digitalInputs.discover()) {
  //   SAFE_PRINTF("[CM4]\t DigitalInputs::discover() returned false\r\n");
  //   return ResultCode::ERR_SETTING_DIGITAL_INPUT_LINE_CONFIG;
  // }

  digitalInputs.connectSettingUpdateSink(radioSettings);

  rc = digitalInputs.open();
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t DigitalInputs::open() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }
  return rc;
  // digitalInputs.emplace_back(digitalInput);
  // digitalInputs.emplace_back(rotaryEncoder);
  // SAFE_PRINTF("[CM4]\t About call DigitalInput::configure()\r\n");
  // rc = digitalInputs.at(0).configure(digitalInputConfig);
  // if (rc != ResultCode::OK) {
  //   SAFE_PRINTF("[CM4]\t DigitalInput::configure() returned %d\r\n", static_cast<int>(rc));
  //   return rc;
  // }
  //
  // SAFE_PRINTF("[CM4]\t About call RotaryEncoder::configure()\r\n");
  // rc = digitalInputs.at(1).configure(rotaryEncoderConfig);
  // if (rc != ResultCode::OK) {
  //   SAFE_PRINTF("[CM4]\t RotaryEncoder::configure() returned %d\r\n", static_cast<int>(rc));
  //   return rc;
  // }
  // GpioInputLinesRequestVector inputs;
  // for (auto& input : digitalInputs) {
  //   inputs.emplace_back(&input.getLinesRequest());
  // }
  // SAFE_PRINTF("[CM4]\t About call GpioInputLinesRequest::request()\r\n");
  //
  // rc = linesSource.requestLines("", inputs);
  // if (rc != ResultCode::OK) {
  //   SAFE_PRINTF("[CM4]\t GpioInputLinesRequest::request() returned %d\r\n", static_cast<int>(rc));
  //   return rc;
  // }
  // SAFE_PRINTF("[CM4]\t About to start GpioInputLinesRequest thread\r\n");
  // return linesSource.start(configMINIMAL_STACK_SIZE*5, tskIDLE_PRIORITY, "gpio");
}