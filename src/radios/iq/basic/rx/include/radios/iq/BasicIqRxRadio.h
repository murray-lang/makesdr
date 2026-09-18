#pragma once

#include <ResultCode.h>
#include <iq/rx/BasicIqRx.h>
#include <config/struct/RadioConfig.h>
#include <settings/control/radio/RadioControlT.h>
#include <settings/model/radios/iq/BasicIqRxSettings.h>
#include <settings/model/data/radio/RadioLookup.h>

#include <radios/base/RadioBaseT.h>

class BasicIqRxRadio : public RadioBaseT<BasicIqRxSettings>
{
public:
  BasicIqRxRadio(
    const BandCategoryList& bands,
    const ModeList& modes,
    BasicIqRxSettings::Cache& bandSettingsCache
    );

  ResultCode configure(const Config::Radio::Fields& radioConfig) override;

  ResultCode start() override;
  void stop() override;

  ResultCode applySettings(BasicIqRxSettings& settings) override;
  ResultCode applyFieldUpdate(const FieldUpdate& update) override;


  ResultCode ptt(bool on) override;

protected:
  void pttOn();
  void pttOff();

protected:
  BasicIqRx m_receiver;
  RadioControlT<BasicIqRxSettings> m_control;
  BasicIqRxSettings m_settings;
};
