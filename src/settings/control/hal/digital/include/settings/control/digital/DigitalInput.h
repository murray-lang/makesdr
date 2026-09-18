#pragma once

#include <gpio/input/GpioInputLines.h>
#include <gpio/input/GpioInputLinesRequest.h>

#include <settings/model/message/FieldUpdate.h>
#include <config/struct/DigitalInputConfig.h>

#include <settings/model/message/FieldDescriptor.h>
#include <settings/model/message/FieldUpdateSource.h>

// #ifdef USE_DOTTED_STRING_PATHS
#include <settings/model/message/ResolveDottedStringFunc.h>
// #endif


#ifdef USE_ETL
#include "etl/string.h"
using IdString = etl::string<MAX_ID_LENGTH>;
#else
#include <string>

using IdString = std::string;
#endif

class DigitalInput : public GpioInputLines, public FieldUpdateSource
{
public:

  explicit DigitalInput();
  ~DigitalInput() override = default;

  // DigitalInput(DigitalInput& rhs)  noexcept;
  // DigitalInput& operator=(const DigitalInput& rhs)  noexcept;

  DigitalInput(DigitalInput&& rhs)  noexcept;
  DigitalInput& operator=(DigitalInput&& rhs)  noexcept;
  virtual ResultCode configure(const Config::DigitalInput::Fields& config, ResolveDottedStringFunc resolver);
  [[nodiscard]] const IdString& getId() const { return m_id; }
  [[nodiscard]] const FieldPath& getFieldPath() const { return m_settingDescriptor.getPath(); }

  GpioInputLinesRequest& getLinesRequest() { return m_linesRequest; }

  void connectFieldUpdateSink(FieldUpdateSink* sink) override;
protected:
  void handleGpioLineEvent(GpioLineEvent* event);

  ResultCode notifyFieldUpdate(const FieldUpdate& settingDelta) override;

  IdString m_id;
  GpioLineEventCallback m_lineEventCallback;
  GpioInputLinesRequest m_linesRequest;
  FieldDescriptor m_settingDescriptor;
  FieldUpdateSink* m_pSink{nullptr};
};
