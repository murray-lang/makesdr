#pragma once
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>
#include <settings/model/data/mode/ModeList.h>
#include <settings/model/data/band/BandCategoryList.h>
#include <radios/base/RadioBaseT.h>
#include <settings/model/message/FieldUpdateMessage.h>
// #include <transport/in-out/QtTransportInOutT.h>
#include <transport/radio/QtRadioTransportT.h>
#include <settings/model/radios/RadioSettingsRequester.h>


class QtTransportTestRadio :
  public RadioBaseT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<SplitBandDualIqRxTxSettings>,
  public MessageSinkT<FieldUpdateMessage>
{
public:
  QtTransportTestRadio();
  ~QtTransportTestRadio() override;

  ResultCode configure();
  ResultCode configure(const Config::Radio::Fields& config) override;
  ResultCode start() override;
  void stop() override;

  [[nodiscard]] const IRadioSettings* getSettings() const  override { return &m_settings; }

  ResultCode applyMessage(SplitBandDualIqRxTxSettings* message) final;
  ResultCode applyMessage(FieldUpdateMessage* update) final;

  ResultCode applySettings(SplitBandDualIqRxTxSettings& settings) final;

  ResultCode applyFieldUpdate(const FieldUpdate& update) final;

  ResultCode ptt(bool on) final;

  ResultCode sendSettings();
  ResultCode sendModes();
  ResultCode sendBands();

protected:
  ResultCode handleRequest(RadioSettingsRequester::Requests cmd);

private:
  QtRadioTransportT<SplitBandDualIqRxTxSettings> m_transport;
  // QThread m_thread;
  // QtTransportInOutT<SplitBandDualIqRxTxSettings> m_settingsTransport;
  // QtTransportInOutT<FieldUpdateMessage> m_updateTransport;
  // QtTransportInOutT<ModeList> m_modesTransport;
  // QtTransportInOutT<BandCategoryList> m_bandsTransport;
  SplitBandDualIqRxTxSettings m_settings;
  RadioSettingsUpdater* m_pShortcuts;
};
