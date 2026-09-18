#pragma once

#include <ResultCode.h>
#include <iq/split/SplitBandDualIq.h>
#include <config/struct/RadioConfig.h>
// #include <settings/model/radio/BandSettingsCache.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>

#include <radios/base/RadioBaseT.h>
#include <settings/control/radio/RadioControlT.h>

#include <thread/Mutex.h>

#include <settings/model/radios/RadioSettingsRequester.h>

#ifdef IS_LINUX
#include <transport/radio/QtRadioTransportT.h>
#endif

class SplitBandDualIqRadio :
  public RadioBaseT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<FieldUpdateMessage>
{

public:
  SplitBandDualIqRadio(
    BandCategoryList& bands,
    ModeList& modes,
    SplitBandDualIqRxTxSettings::Cache& bandSettingsCache
    );

  ResultCode configure(const Config::Radio::Fields& config) override;

  void setSettings(SplitBandDualIqRxTxSettings& settings)
  {
    m_settings.replace(settings, false);
    m_settings.autoComplete();
    m_settings.deemComplete(true);
  }

  ResultCode start() override;
  void stop() override;

  ResultCode applyMessage(SplitBandDualIqRxTxSettings* message) final;
  ResultCode applyMessage(FieldUpdateMessage* update) final;

  [[nodiscard]] const IRadioSettings* getSettings() const  override { return &m_settings; }
  // [[nodiscard]] const RadioLookup* getLookup() const override { return &m_lookup; }

  ResultCode applySettings(SplitBandDualIqRxTxSettings& settings) override;
  ResultCode applyFieldUpdate(const FieldUpdate& update) override;

  ResultCode ptt(bool on) override;

protected:
  // ResultCode applyFieldUpdate(const FieldUpdate& update, PayloadSource source);
  ResultCode processCurrentSettings();
  ResultCode handleRequest(RadioSettingsRequester::Requests cmd);
  ResultCode sendSettings();
  ResultCode sendModes();
  ResultCode sendBands();

protected:
  // const RadioLookup& m_lookup;
  BandCategoryList& m_bands;
  ModeList& m_modes;
  SplitBandDualIq m_transceiver;
  RadioControlT<SplitBandDualIqRxTxSettings> m_control;
  Mutex m_settingsMutex;
  SplitBandDualIqRxTxSettings m_settings;
  RadioSettingsUpdater* m_pUpdater;

#ifdef IS_QT
  QtRadioTransportT<SplitBandDualIqRxTxSettings> m_transport;
#endif
};
