//
// Created by murray on 2025-08-24.
//

#pragma once
#include "model/SettingFieldUpdate.h"
#include "config/DigitalInputConfig.h"
#include "model/SettingFieldPath.h"
#include "model/SettingFieldUpdateSource.h"

#include "gpio/DigitalInputLinesRequest.h"
#include "gpio/base/GpioInputLines.h"
#include "gpio/DigitalInputLinesRequest.h"

#ifdef USE_ETL
#include "etl/string.h"
using IdString = etl::string<MAX_ID_LENGTH>;
#else
#include <string>

using IdString = std::string;
#endif

class DigitalInput : public GpioInputLines, public SettingFieldUpdateSource
{
public:

  explicit DigitalInput();
  ~DigitalInput() override = default;

  DigitalInput(DigitalInput&&)  noexcept = default;
  DigitalInput& operator=(DigitalInput&&)  noexcept = default;

  virtual ResultCode configure(const Config::DigitalInput::Fields& config);
  // void setId(const std::string& id) { m_id = id; }
  [[nodiscard]] const IdString& getId() const { return m_id; }
  [[nodiscard]] bool getDetectEdge() const { return m_detectEdge; }
  // [[nodiscard]] const GpioLines& getLines() const { return m_lines; }
  [[nodiscard]] const SettingFieldPath& getSettingFieldPath() const { return m_settingPath; }

  virtual bool handleLineChange(DigitalInputLinesRequest::LineStateVector& changedLines);

  void connectSettingFieldUpdateSink(SettingFieldUpdateSink& sink) override;
protected:
  void notifyChange(const DigitalInputLinesRequest::LineState& lineState);
  ResultCode notifySettingFieldUpdate(const SettingFieldUpdate& settingDelta) override;

  IdString m_id;
  bool m_detectEdge;
  SettingFieldPath m_settingPath;
  optional<reference_wrapper<SettingFieldUpdateSink>> m_pSink;
};
