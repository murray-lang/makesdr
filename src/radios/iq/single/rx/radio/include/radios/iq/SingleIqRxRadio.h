#pragma once

#include <ResultCode.h>
#include <radios/iq/SingleIqRxT.h>
#include <config/struct/RadioConfig.h>
// #include <settings/model/radio/BandSettingsCache.h>
#include <settings/model/radio/iq/SingleIqRxSettings.h>

#include <radios/base/RadioBase.h>
#include <settings/control/radio/RadioControlT.h>
#include <thread/Mutex.h>
#include <settings/model/radio/RadioRequester.h>
#include <settings/model/message/MessageSinkT.h>

#ifdef IS_QT
#include <transport/qt/QtIqCapableRadioTransportT.h>
#include <transport/qt/globalQtSingleIqRxRings.h>

using SingleIqRxTransport = QtIqCapableRadioTransportT<QtSingleIqRxSettingsRings>;
#elif defined(IS_STM32)
#include <transport/stm32h745i/Stm32H745IqCapableRadioTransportT.h>
#include <transport/stm32h745i/globalStm32H745SingleIqRxRings.h>

using SingleIqRxTransport = Stm32H745IqCapableRadioTransportT<Stm32H745SingleIqRxSettingsRings>;
#endif

class SingleIqRxRadio :
  public RadioBase,
  public RadioSettingsSinkT<SingleIqRxSettings>,
  public MessageSinkT<SingleIqRxSettings>,
  public MessageSinkT<FieldUpdateMessage>
{
public:
  using Settings = SingleIqRxSettings;

  SingleIqRxRadio(
    BandCategoryList& bands,
    ModeList& modes,
    SingleIqRxSettings::Cache& bandSettingsCache
    );

  ResultCode configure(const Config::Radio::Fields& radioConfig) override;

  void setSettings(Settings& settings)
  {
    m_settings.replace(settings, false);
    m_settings.autoComplete();
    m_settings.deemComplete(true);
  }

  ResultCode applyMessage(Settings* message) final;
  ResultCode applyMessage(FieldUpdateMessage* update) final;

  [[nodiscard]] const Settings* getSettings() const  { return &m_settings; }

  ResultCode start() override;
  void stop() override;

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

protected:
  SingleIqRxTransport m_transport;
  ComplexPingPongBuffers m_pingPongBuffers;
  BandCategoryList& m_bands;
  ModeList& m_modes;
  SingleIqRxT<SingleIqRxSettings> m_receiver;
  RadioControlT<SingleIqRxSettings> m_control;
  Mutex m_settingsMutex;
  Settings m_settings;
  RadioSettingsUpdater* m_pUpdater;
};
