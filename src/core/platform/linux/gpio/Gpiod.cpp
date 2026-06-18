
#include <gpiod.h>

#include "settings/control/gpio/base/Gpio.h"
#include "settings/control/gpio/base/GpioInputLinesRequest.h"

constexpr static auto defaultChipPath = "/dev/gpiochip0";

Gpio::Gpio() :
    m_pChip(nullptr)
{
    
}

Gpio::~Gpio()
{
  close();
}
    
bool
Gpio::isPresent()
{
    gpiod_chip* pChip = gpiod_chip_open(defaultChipPath);
    if (pChip == nullptr) {
        return false;
    }
    gpiod_chip_close(pChip);
    return true;
}
ResultCode
Gpio::open()
{
  m_pChip = gpiod_chip_open(defaultChipPath);
  if (m_pChip == nullptr) {
   return ResultCode::ERR_SETTING_CONTROL_GPIO_CHIP_OPEN;;
  }
  return ResultCode::OK;
}

void
Gpio::close()
{
  if (m_pChip) {
    gpiod_chip_close(static_cast<gpiod_chip*>(m_pChip));
    m_pChip = nullptr;
  }
}


// ResultCode
// Gpio::requestDigitalInputs(
//     const char * contextId,
//     const GpioLinesVector& inputs,
//     GpioLinesRequestBase& lr
//   )
// {
//   if (!m_pGpioHandle) {
//     return ResultCode::ERR_SETTING_CONTROL_GPIOD_CHIP_NOT_OPENED;
//   }
//   gpiod_line_config *lcfg = gpiod_line_config_new();
//   if (lcfg == nullptr) {
//     return ResultCode::ERR_DIGITAL_INPUT_LINE_CONFIG;
//   }
//   for (const auto* input : inputs) {
//     gpiod_line_settings *ls = gpiod_line_settings_new();
//     gpiod_line_settings_set_direction(ls, GPIOD_LINE_DIRECTION_INPUT);
//     gpiod_line_settings_set_bias(ls, static_cast<gpiod_line_bias>(input->getBias()));
//     gpiod_line_settings_set_edge_detection(ls, static_cast<gpiod_line_edge>(input->getEdge()));
//     // gpiod_line_settings_set_debounce_period_us(ls, 2000);
//     const GpioLineNumberVector& lineNos = input->getLines();
//     gpiod_line_config_add_line_settings(lcfg, lineNos.data(), lineNos.size(), ls);
//     gpiod_line_settings_free(ls);
//   }
//
//   gpiod_request_config *rcfg = gpiod_request_config_new();
//   gpiod_request_config_set_consumer(rcfg, contextId);
//
//   gpiod_line_request *pLineRequest = gpiod_chip_request_lines(static_cast<gpiod_chip*>(m_pGpioHandle), rcfg, lcfg);
//   gpiod_request_config_free(rcfg);
//   gpiod_line_config_free(lcfg);
//   if (pLineRequest == nullptr) {
//     return ResultCode::ERR_DIGITAL_INPUT_LINE_REQUEST_FAILED;
//   }
//   m_pLineRequest = pLineRequest;
//   initialiseLineStates(inputs);
//   return ResultCode::OK;
// }

// ResultCode
// Gpio::requestDigitalOutputs(
//   const char * contextId,
//   const GpioLineNumberVector& outputs,
//   GpioLinesRequestBase& lr
// )
// {
//   if (!m_pGpioHandle) {
//     return ResultCode::ERR_SETTING_CONTROL_GPIOD_CHIP_NOT_OPENED;
//   }
//
// }

ResultCode
Gpio::requestInputs(
  const char * contextId,
  const GpioInputLinesVector& inputs,
  GpioInputLinesRequest& dilr
)
{
  if (!m_pChip) {
    return ResultCode::ERR_SETTING_CONTROL_GPIOD_CHIP_NOT_OPENED;
  }
  dilr.initialise(m_pChip, contextId);
  return dilr.request(contextId, inputs);
}

ResultCode
Gpio::requestOutput(
  const char * contextId,
  const GpioLines& output,
  GpioOutputLinesRequest& dolr
)
{
  if (!m_pChip) {
    return ResultCode::ERR_SETTING_CONTROL_GPIOD_CHIP_NOT_OPENED;
  }
  dolr.initialise(m_pChip, contextId);
  return dolr.request(contextId, output);
}
