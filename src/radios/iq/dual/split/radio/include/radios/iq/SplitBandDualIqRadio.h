#pragma once

#include <ResultCode.h>
#include <radios/iq/DualIqRxTxT.h>
#include <config/struct/RadioConfig.h>
// #include <settings/model/radio/BandSettingsCache.h>
#include <radios/settings/model/SplitBandDualIqRxTxSettings.h>

#include <radios/base/RadioBaseT.h>
#include <settings/control/radio/RadioControlT.h>

#include <thread/Mutex.h>

#include <settings/model/radio/RadioSettingsRequester.h>

#ifdef IS_QT
#include <radios/iq/SplitBandDualIqTransport.h>
#endif

class DualIqRxTxRadio :
  public RadioBaseT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<FieldUpdateMessage>
{

public:
  DualIqRxTxRadio(
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

  ResultCode applySettings(SplitBandDualIqRxTxSettings& settings) override;
  ResultCode applyFieldUpdate(const FieldUpdate& update) override;

  ResultCode ptt(bool on) override;

protected:
  // ResultCode applyFieldUpdate(const FieldUpdate& update, PayloadSource source);
  ResultCode processCurrentSettings();
  ResultCode handleRequest(RadioSettingsRequester::Requests cmd);
  ResultCode sendCapabilities();
  ResultCode sendSettings();
  ResultCode sendModes();
  ResultCode sendBands();

protected:
#ifdef IS_QT
  SplitBandDualIqTransport m_transport;
#endif

  BandCategoryList& m_bands;
  ModeList& m_modes;
  DualIqRxTxT<SplitBandDualIqRxTxSettings> m_transceiver;
  RadioControlT<SplitBandDualIqRxTxSettings> m_control;
  Mutex m_settingsMutex;
  SplitBandDualIqRxTxSettings m_settings;
  RadioSettingsUpdater* m_pUpdater;


};
