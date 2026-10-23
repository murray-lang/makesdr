#pragma once

#include <ResultCode.h>
#include <radios/iq/DualIqRxTxT.h>
#include <config/struct/RadioConfig.h>
// #include <settings/model/radio/BandSettingsCache.h>
#include <settings/model/radio/iq/DualIqRxTxSettings.h>

#include <radios/base/RadioBase.h>
#include <settings/control/radio/RadioControlT.h>

#include <thread/Mutex.h>

#include <settings/model/radio/RadioRequester.h>

#ifdef IS_QT
#include <transport/qt/QtIqCapableRadioTransportT.h>
#include <transport/qt/globalQtDualIqRxTxRings.h>

using DualIqRxTxTransport = QtIqCapableRadioTransportT<QtDualIqRxTxSettingsRings>;
#endif

class DualIqRxTxRadio :
  public RadioBase<DualIqRxTxSettings>,
  public MessageSinkT<DualIqRxTxSettings>,
  public MessageSinkT<FieldUpdateMessage>
{

public:

  using Settings = DualIqRxTxSettings;

  DualIqRxTxRadio(
    BandCategoryList& bands,
    ModeList& modes,
    DualIqRxTxSettings::Cache& bandSettingsCache
    );

  ResultCode configure(const Config::Radio::Fields& config) override;

  void setSettings(DualIqRxTxSettings& settings)
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
  // ResultCode applyFieldUpdate(const FieldUpdate& update, PayloadSource source);
  ResultCode processCurrentSettings();
  ResultCode handleRequest(RadioRequester::Requests cmd);
  ResultCode sendSettings();
  ResultCode sendModes();
  ResultCode sendBands();

protected:
  DualIqRxTxTransport m_transport;

  BandCategoryList& m_bands;
  ModeList& m_modes;
  DualIqRxTxT<Settings> m_transceiver;
  RadioControlT<Settings> m_control;
  Mutex m_settingsMutex;
  Settings m_settings;
  RadioSettingsUpdater* m_pUpdater;


};
