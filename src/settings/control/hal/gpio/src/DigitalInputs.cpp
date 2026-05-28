#include <CrossPlatformTypes.h>
#include "gpio/DigitalInputs.h"
// #include "../../../ControlSourceFactory.h"
// #include <qdebug.h>

#include "gpio/DigitalInputFactory.h"
// #include "gpio/GpioInputLinesVector.h"

DigitalInputs::DigitalInputs(const char* consumer) :
  m_internalSink(*this)
{

}

DigitalInputs::~DigitalInputs()
{
  m_lineToInputMap.clear();
}

DigitalInputs::DigitalInputs(DigitalInputs&& rhs) noexcept :
  SettingsControlSource(move(rhs)),
  m_internalSink(*this),  // Reference to new object
  m_inputs(move(rhs.m_inputs)),
  m_linesRequest(move(rhs.m_linesRequest)),
  m_lineToInputMap(move(rhs.m_lineToInputMap))
{
  reconnectInputSinks();
}

DigitalInputs& DigitalInputs::operator=(DigitalInputs&& rhs) noexcept
{
  if (this != &rhs) {
    SettingsControlSource::operator=(move(rhs));
    m_inputs = move(rhs.m_inputs);
    m_linesRequest = move(rhs.m_linesRequest);
    m_lineToInputMap = move(rhs.m_lineToInputMap);
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
  if (!m_linesRequest.has_value()) {
    createLineToInputMap();
    Gpio& gpio = Gpio::getInstance();
    // DigitalInputLinesRequest lines;
    m_linesRequest.emplace();
    GpioInputLinesVector inputs;
    for (auto& input : m_inputs) {
      visit([&inputs](auto&& di)
      {
        inputs.emplace_back(move(di));
      }, input);

    }
    ResultCode rc = gpio.requestInputs("digitalInputs", inputs, *m_linesRequest);
    if (rc != ResultCode::OK) {
      return rc;
    }
    m_linesRequest->startCallbacks(*this);
  }
  return ResultCode::OK;
}

void
DigitalInputs::close()
{
  if (m_linesRequest) {
    m_linesRequest->stopCallbacks();
    m_linesRequest->release();
    m_linesRequest.reset();
  }
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
    DigitalInputVariant digitalInput;
    rc = DigitalInputFactory::create(inputConfig, digitalInput);
    if (rc == ResultCode::OK) {
      visit([this, rc](auto&& di) {
        di.connectSettingFieldUpdateSink(m_internalSink);
      }, digitalInput);
      m_inputs.emplace_back(move(digitalInput));
    } else {
      break;
    }
  }
  return rc;
}

void
DigitalInputs::createLineToInputMap()
{
  m_lineToInputMap.clear();
  for (auto& input : m_inputs) {
    visit([this, &input](auto&& di) {
      const GpioLineNumberVector& lineNos = di.getLines();
      for (const auto& lineNo : lineNos) {
        m_lineToInputMap.insert(make_pair(lineNo, ref(input)));
// #ifdef USE_ETL
//         m_lineToInputMap.insert(make_pair(lineNo, ref(input)));
// #else
//         m_lineToInputMap[lineNo] = move(input);
// #endif
      }
    }, input);
  }
}

void
DigitalInputs::reconnectInputSinks()
{
  for (auto& input : m_inputs) {
    visit([this](auto&& di) {
      di.connectSettingFieldUpdateSink(m_internalSink);
    }, input);
  }
}

void
DigitalInputs::readInitialInputStates()
{
  // for (auto encoder : m_encoders) {
  //   encoder->initialiseState(m_lines);
  // }
}

void
DigitalInputs::callback(DigitalInputLinesRequest::LineStateVector& lineStates)
{
  for (auto& input : m_inputs) {
    visit([&lineStates](auto&& di) {
      di.handleLineChange(lineStates);
    }, input);
  }
}
