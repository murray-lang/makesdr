#include "gpioTest.h"
#include <etl/vector.h>
#include "digitalInputConfigs.h"
#include <gpio/service/GpioInputLinesSource.h>
#include <stm32h745i/app/support/safe_printf.h>
#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>

class DigitalInputParent : public GpioInputLines
{
public:
  DigitalInputParent()
    : m_lineEventCallback(
      GpioLineEventCallback::create<DigitalInputParent, &DigitalInputParent::handleGpioLineEvent>(*this)
      )
    , m_linesRequest(*this, m_lineEventCallback)
  {
  }
  DigitalInputParent(const DigitalInputParent& rhs)
    : DigitalInputParent()
  {
  }

  GpioInputLinesRequest& getLinesRequest() { return m_linesRequest; }

  ResultCode configure(const Config::DigitalInput::Fields& config)
  {
    return configureLines(config);
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
protected:
  GpioLineEventCallback m_lineEventCallback;
  GpioInputLinesRequest m_linesRequest;

};

GpioInputLinesSource linesSource;

DigitalInputParent digitalInput;
DigitalInputParent rotaryEncoder;

etl::vector<DigitalInputParent, 2> digitalInputs; //{ digitalInputVariant, rotaryEncoderVariant};


ResultCode gpioTest()
{
  digitalInputs.emplace_back(digitalInput);
  digitalInputs.emplace_back(rotaryEncoder);
  SAFE_PRINTF("[CM4]\t About call DigitalInput::configure()\r\n");
  ResultCode rc = digitalInputs.at(0).configure(digitalInputConfig);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t DigitalInput::configure() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }

  SAFE_PRINTF("[CM4]\t About call RotaryEncoder::configure()\r\n");
  rc = digitalInputs.at(1).configure(rotaryEncoderConfig);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t RotaryEncoder::configure() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }
  GpioInputLinesRequestVector inputs;
  for (auto& input : digitalInputs) {
    inputs.emplace_back(&input.getLinesRequest());
  }
  SAFE_PRINTF("[CM4]\t About call GpioInputLinesRequest::request()\r\n");

  rc = linesSource.requestLines("", inputs);
  if (rc != ResultCode::OK) {
    SAFE_PRINTF("[CM4]\t GpioInputLinesRequest::request() returned %d\r\n", static_cast<int>(rc));
    return rc;
  }
  SAFE_PRINTF("[CM4]\t About to start GpioInputLinesRequest thread\r\n");
  return linesSource.start(configMINIMAL_STACK_SIZE*5, tskIDLE_PRIORITY, "gpio");
}