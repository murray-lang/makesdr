#pragma once
#include "settings/model/core/SettingUpdate.h"
#include "config/struct/DigitalInputConfig.h"
#include "settings/model/core/SettingPath.h"
#include "settings/model/core/SettingUpdateSource.h"

#include "settings/control/gpio/DigitalInputLinesRequest.h"
#include "settings/control/gpio/base/GpioInputLinesConfig.h"
#include "settings/control/gpio/DigitalInputLinesRequest.h"

#ifdef USE_ETL
#include "etl/string.h"
using IdString = etl::string<MAX_ID_LENGTH>;
#else
#include <string>

using IdString = std::string;
#endif

class DigitalInput : public GpioInputLinesConfig, public SettingUpdateSource
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
  [[nodiscard]] const SettingPath& getSettingPath() const { return m_settingPath; }

  virtual bool handleLineChange(DigitalInputLinesRequest::LineStateVector& changedLines);

  void connectSettingUpdateSink(SettingUpdateSink& sink) override;
protected:
  void notifyChange(const DigitalInputLinesRequest::LineState& lineState);
  ResultCode notifySettingUpdate(const SettingUpdate& settingDelta) override;

  IdString m_id;
  bool m_detectEdge;
  SettingPath m_settingPath;
  bool m_isPathIndirect;
  AutoCompleteTrigger m_autoCompleteTrigger;
  optional<reference_wrapper<SettingUpdateSink>> m_pSink;
};
