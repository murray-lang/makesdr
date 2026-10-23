#pragma once

#include <ResultCode.h>
#include <radios/iq/DualIqRxT.h>
#include <config/struct/RadioConfig.h>
// #include <settings/model/radio/BandSettingsCache.h>
#include <settings/model/radio/iq/DualIqRxSettings.h>

#include <radios/base/RadioBase.h>
#include <settings/control/radio/RadioControlT.h>
#include <thread/Mutex.h>
#include <settings/model/radio/RadioRequester.h>
#include <settings/model/message/MessageSinkT.h>

#ifdef IS_QT
#include <transport/qt/QtIqCapableRadioTransportT.h>
#include <transport/qt/globalQtDualIqRxRings.h>

using DualIqRxTransport = QtIqCapableRadioTransportT<QtDualIqRxSettingsRings>;
#endif

class DualIqRxRadio :
  public RadioBase<DualIqRxSettings>,
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

  void setSettings(Settings& settings)
  {
    m_settings.replace(settings, false);
    m_settings.autoComplete();
    m_settings.deemComplete(true);
  }

  ResultCode start() override;
  void stop() override;

  ResultCode applyMessage(Settings* message) final;
  ResultCode applyMessage(FieldUpdateMessage* update) final;

  [[nodiscard]] const Settings* getSettings() const  override { return &m_settings; }

  ResultCode applySettings(Settings& settings) override;
  ResultCode applyFieldUpdate(const FieldUpdate& update) override;

  ResultCode ptt(bool on) override;

protected:
  ResultCode processCurrentSettings();
  ResultCode handleRequest(RadioRequester::Requests cmd);
  ResultCode sendSettings();
  ResultCode sendModes();
  ResultCode sendBands();

protected:
  DualIqRxTransport m_transport;
  ComplexPingPongBuffers m_pingPongBuffers;
  BandCategoryList& m_bands;
  ModeList& m_modes;
  DualIqRxT<Settings> m_receiver;
  RadioControlT<Settings> m_control;
  Mutex m_settingsMutex;
  Settings m_settings;
  RadioSettingsUpdater* m_pUpdater;


};
