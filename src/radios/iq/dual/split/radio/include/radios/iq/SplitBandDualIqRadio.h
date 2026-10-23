#pragma once

#include <ResultCode.h>
#include <radios/iq/DualIqRxTxT.h>
#include <config/struct/RadioConfig.h>
#include <settings/model/radio/iq/SplitBandDualIqRxTxSettings.h>

#include <radios/base/RadioBase.h>
#include <settings/control/radio/RadioControlT.h>

#include <thread/Mutex.h>

#include <settings/model/radio/RadioRequester.h>

#ifdef IS_QT
#include <transport/qt/QtIqCapableRadioTransportT.h>
#include <transport/qt/globalQtDualIqSplitRings.h>

using SplitBandDualIqTransport = QtIqCapableRadioTransportT<QtSplitBandDualIqSettingsRings>;
#endif

class SplitBandDualIqRxTxRadio :
  public RadioBase,
  public RadioSettingsSinkT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<FieldUpdateMessage>
{

public:
  SplitBandDualIqRxTxRadio(
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

  [[nodiscard]] const SplitBandDualIqRxTxSettings* getSettings() const { return &m_settings; }

  ResultCode applySettings(SplitBandDualIqRxTxSettings& settings) override;
  ResultCode applyFieldUpdate(const FieldUpdate& update) override;

  ResultCode ptt(bool on) override;

protected:
  // ResultCode applyFieldUpdate(const FieldUpdate& update, PayloadSource source);
  ResultCode processCurrentSettings();
  ResultCode handleRequest(RadioRequester::Requests cmd);
  ResultCode sendCapabilities();
  ResultCode sendSettings();
  ResultCode sendModes();
  ResultCode sendBands();

protected:
  CapabilitiesMessage m_capabilitiesMsg;
  SplitBandDualIqTransport m_transport;

  BandCategoryList& m_bands;
  ModeList& m_modes;
  DualIqRxTxT<SplitBandDualIqRxTxSettings> m_transceiver;
  RadioControlT<SplitBandDualIqRxTxSettings> m_control;
  Mutex m_settingsMutex;
  SplitBandDualIqRxTxSettings m_settings;
  RadioSettingsUpdater* m_pUpdater;


};
