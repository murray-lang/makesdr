#include "gpioTest.h"
#include <gpio/GpioDigitalInput.h>
#include <gpio/GpioRotaryEncoder.h>
#include <../../../src/core/platform/stm32h745i/gpio/include/gpio/stm32h745i/GpioInputLinesRequest_old.h>
#include <gpio/common/GpioLineEventCallback.h>
#include <etl/vector.h>
#include "digitalInputConfigs.h"
#include "stm32h745i/app/support/safe_printf.h"

class DigitalInputParent : public GpioDigitalInput
{
public:
  DigitalInputParent()
    : GpioDigitalInput(
        GpioLineEventCallback::create<DigitalInputParent, &DigitalInputParent::handleGpioLineEvent>(*this)
      )
  {
  }

  void handleGpioLineEvent(GpioLineEvent* event)
  {
    // if (event->value > 0) {
    //   BSP_LED_Toggle(LED_GREEN);
    // } else {
    //   BSP_LED_Toggle(LED_RED);
    // }
  }
};


class RotaryEncoderParent : public GpioRotaryEncoder
{
public:
  RotaryEncoderParent()
    : GpioRotaryEncoder(
        GpioLineEventCallback::create<RotaryEncoderParent, &RotaryEncoderParent::handleGpioLineEvent>(*this)
      )
  {
  }

  void handleGpioLineEvent(GpioLineEvent* event)
  {
    if (event->value > 0) {
      BSP_LED_On(LED_GREEN);
      BSP_LED_Off(LED_RED);
    } else if (event->value < 0) {
      BSP_LED_On(LED_RED);
      BSP_LED_Off(LED_GREEN);
    } else {
      BSP_LED_Off(LED_RED);
      BSP_LED_Off(LED_GREEN);
    }
  }
};

using DigitalVariant = etl::variant<DigitalInputParent, RotaryEncoderParent>;
using DigitalVector = etl::vector<DigitalVariant, 2>;

DigitalInputParent digitalInputParent;
DigitalInputParent digitalInputParent2;
RotaryEncoderParent rotaryEncoderParent;

// DigitalVariant digitalInputVariant = digitalInputParent;
// DigitalVariant rotaryEncoderVariant = rotaryEncoderParent;

DigitalVector digitalInputs; //{ digitalInputVariant, rotaryEncoderVariant};

GpioInputLinesRequest_old linesRequest;

ResultCode gpioTest()
{
  // digitalInputs.emplace_back(digitalInputVariant);
  // digitalInputs.emplace_back(rotaryEncoderVariant);
  digitalInputs.emplace_back(etl::in_place_type<DigitalInputParent>, digitalInputParent);
  // digitalInputs.emplace_back(etl::in_place_type<DigitalInputParent>, digitalInputParent2);
  digitalInputs.emplace_back(etl::in_place_type<RotaryEncoderParent>, rotaryEncoderParent);
  // digitalInputs.push_back(DigitalVariant(digitalInputParent));
  // digitalInputs.push_back(DigitalVariant(rotaryEncoderParent));
  SAFE_PRINTF("[CM4]\t About call DigitalInput::configure()\r\n");
  ResultCode rc = etl::get<DigitalInputParent>(digitalInputs.at(0)).configure(digitalInputConfig);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t DigitalInput::configure() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }

  SAFE_PRINTF("[CM4]\t About call RotaryEncoder::configure()\r\n");
  rc = etl::get<RotaryEncoderParent>(digitalInputs.at(1)).configure(rotaryEncoderConfig);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t RotaryEncoder::configure() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }
  GpioDigitalInputVector inputs;
  for (auto& input : digitalInputs) {
    etl::visit([&inputs](auto&& di)
    {
      inputs.emplace_back(&di);
    }, input);
  }
  SAFE_PRINTF("[CM4]\t About call GpioInputLinesRequest::request()\r\n");
  rc = linesRequest.request("", inputs);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t GpioInputLinesRequest::request() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }
  SAFE_PRINTF("[CM4]\t About to start GpioInputLinesRequest thread\r\n");
  return linesRequest.start(configMINIMAL_STACK_SIZE*5, tskIDLE_PRIORITY, "gpio");
}