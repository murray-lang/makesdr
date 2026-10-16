#pragma once

#include <ResultCode.h>
#include <radios/iq/DualIqReceiverT.h>
#include <config/struct/RadioConfig.h>
// #include <settings/model/radio/BandSettingsCache.h>
#include <radios/settings/model/DualIqRxSettings.h>

#include <radios/base/RadioBaseT.h>
#include <settings/control/radio/RadioControlT.h>

#include <thread/Mutex.h>

#include <settings/model/radio/RadioSettingsRequester.h>

#ifdef IS_QT
#include <radios/iq/DualIqRxTransport.h>
#endif

class DualIqRxRadio :
  public RadioBaseT<DualIqRxSettings>,
  public MessageSinkT<DualIqRxSettings>,
  public MessageSinkT<FieldUpdateMessage>
{

public:

  using Settings = DualIqRxSettings;

  DualIqRxRadio(
    BandCategoryList& bands,
    ModeList& modes,
    DualIqRxSettings::Cache& bandSettingsCache
    );

  ResultCode configure(const Config::Radio::Fields& config) override;

  void setSettings(DualIqRxSettings& settings)
  {
    m_settings.replace(settings, false);
    m_settings.autoComplete();
    m_settings.deemComplete(true);
  }

  ResultCode start() override;
  void stop() override;

  ResultCode applyMessage(Settings* message) final;
  ResultCode applyMessage(FieldUpdateMessage* update) final;

  [[nodiscard]] const IRadioSettings* getSettings() const  override { return &m_settings; }

  ResultCode applySettings(Settings& settings) override;
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
#ifdef IS_QT
  DualIqRxTransport m_transport;
#endif
  ComplexPingPongBuffers m_pingPongBuffers;
  BandCategoryList& m_bands;
  ModeList& m_modes;
  DualIqReceiverT<Settings> m_receiver;
  RadioControlT<Settings> m_control;
  Mutex m_settingsMutex;
  Settings m_settings;
  RadioSettingsUpdater* m_pUpdater;


};
