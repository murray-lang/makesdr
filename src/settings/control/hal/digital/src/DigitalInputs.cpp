#include <CrossPlatformTypes.h>
#include <settings/control/digital/DigitalInputs.h>
#include <settings/control/digital/DigitalInputFactory.h>
#include <gpio/service/Gpio.h>

DigitalInputs::DigitalInputs(const char* consumer) :
  m_internalSink(*this)
{

}

DigitalInputs::~DigitalInputs()
{
  // m_lineToInputMap.clear();
}

DigitalInputs::DigitalInputs(DigitalInputs&& rhs) noexcept
  : SettingsControlSource(move(rhs))
  , m_internalSink(*this) // Reference to new object
  , m_inputs(move(rhs.m_inputs))
  // m_linesRequest(move(rhs.m_linesRequest)),
  // m_lineToInputMap(move(rhs.m_lineToInputMap))
{
  reconnectInputSinks();
}

DigitalInputs& DigitalInputs::operator=(DigitalInputs&& rhs) noexcept
{
  if (this != &rhs) {
    SettingsControlSource::operator=(move(rhs));
    m_inputs = move(rhs.m_inputs);
    // m_linesRequest = move(rhs.m_linesRequest);
    // m_lineToInputMap = move(rhs.m_lineToInputMap);
    reconnectInputSinks();
  }
  return *this;
}

ResultCode
DigitalInputs::configure(const Config::DigitalInputs::Fields& config)
{
  return createInputs(config);
}

bool
DigitalInputs::discover()
{
    return Gpio::isPresent();
}

ResultCode
DigitalInputs::open()
{
  // return static_cast<ResultCode>(m_inputs.at(1).getLines());
  GpioInputLinesRequestVector inputs;
  for (auto& input : m_inputs) {
    inputs.emplace_back(&input.getLinesRequest());
  }
  // if (!inputs.at(1)->config.isRotaryEncoder()) {
  //   return static_cast<ResultCode>(1234);
  // }
  // return static_cast<ResultCode>(inputs.at(1)->config.getLines());
  Gpio& gpio = Gpio::getInstance();
  ResultCode rc = gpio.requestInputs("digitalInputs", inputs);
  if (rc != ResultCode::OK) {
    return rc;
  }
  return gpio.startInputs();
}

void
DigitalInputs::close()
{
}

void
DigitalInputs::exit()
{

}

ResultCode
DigitalInputs::createInputs(const Config::DigitalInputs::Fields& config)
{
  m_inputs.clear();
  ResultCode rc = ResultCode::OK;
  for (const auto& inputConfig : config.inputs) {
    DigitalInput digitalInput;
    rc = DigitalInputFactory::create(inputConfig, digitalInput);
    if (rc == ResultCode::OK) {
      digitalInput.connectSettingUpdateSink(m_internalSink);
      m_inputs.emplace_back(move(digitalInput));
    } else {
      break;
    }
  }
  return rc;
}

void
DigitalInputs::reconnectInputSinks()
{
  for (auto& input : m_inputs) {
    input.connectSettingUpdateSink(m_internalSink);
  }
}

