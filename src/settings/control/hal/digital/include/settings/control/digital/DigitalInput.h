#pragma once

#include "gpio/input/GpioInputLines.h"
#include "gpio/input/GpioInputLinesRequest.h"

#include "settings/model/core/SettingUpdate.h"
#include "config/struct/DigitalInputConfig.h"

#include "settings/model/core/SettingPath.h"
#include "settings/model/core/SettingUpdateSource.h"


#ifdef USE_ETL
#include "etl/string.h"
using IdString = etl::string<MAX_ID_LENGTH>;
#else
#include <string>

using IdString = std::string;
#endif

class DigitalInput : public GpioInputLines, public SettingUpdateSource
{
public:

  explicit DigitalInput();
  ~DigitalInput() override = default;

  // DigitalInput(DigitalInput& rhs)  noexcept;
  // DigitalInput& operator=(const DigitalInput& rhs)  noexcept;

  DigitalInput(DigitalInput&& rhs)  noexcept;
  DigitalInput& operator=(DigitalInput&& rhs)  noexcept;

  virtual ResultCode configure(const Config::DigitalInput::Fields& config);
  [[nodiscard]] const IdString& getId() const { return m_id; }
  [[nodiscard]] const SettingPath& getSettingPath() const { return m_settingPath; }

  GpioInputLinesRequest& getLinesRequest() { return m_linesRequest; }

  void connectSettingUpdateSink(SettingUpdateSink& sink) override;
protected:
  void handleGpioLineEvent(GpioLineEvent* event);

  ResultCode notifySettingUpdate(const SettingUpdate& settingDelta) override;

  IdString m_id;
  GpioLineEventCallback m_lineEventCallback;
  GpioInputLinesRequest m_linesRequest;
  SettingPath m_settingPath;
  bool m_isPathIndirect;
  AutoCompleteTrigger m_autoCompleteTrigger;
  optional<reference_wrapper<SettingUpdateSink>> m_pSink;
};
