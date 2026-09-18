#pragma once

#include <ui/qt/faces/FaceBase.h>
#include <memory>

#include <settings/model/radios/iq/RxTxDualIqBandSettings.h>
#include <settings/model/radios/IRadioSettingsUpdater.h>

class QtFrequencyPanel;
class QtTimeSeriesChart;
class QtPanadapter;
class QtSMeter;
class Radio;

namespace Ui { class StandardFace; }

class StandardFace : public FaceBase {
  Q_OBJECT
public:

  using RadioSettings = SplitBandDualIqRxTxSettings;
  using ActiveBandSettings = RadioSettings::ActiveBandSettings;
  using BandSettings = ActiveBandSettings::BandSettings;

  explicit StandardFace(QWidget* parent = nullptr);
  ~StandardFace() override;

  void initialise(const RadioSettings* pRadioSettings, IRadioSettingsUpdater* updater) override;

  // void connect(FieldUpdateSink* pSink) override { m_pExternalSettingsSink = pSink; }
  // void applyFieldUpdate(FieldUpdate& settingDelta) override;

  // void setRadio(Radio* radio) override;
  void handleRadioSettingsChanged(const RadioSettings* pRadioSettings) override;

  // void handleReceiverIq(
  //   RadioSettings* pRadioSettings,
  //   const ComplexSamplesMax* data,
  //   uint32_t length,
  //   uint32_t sampleRate) override;
  // void handleReceiverAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate) override;
  // void handleReceiverMeter(const IqReceiverMetering& metering) override;

  // void handleTransmitterIq(
  //   RadioSettings* pRadioSettings,
  //   const ComplexSamplesMax* data,
  //   uint32_t length,
  //   uint32_t sampleRate) override;
  // void handleTransmitterAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate) override;

protected:

  void updatePanadapter(const BandSettings* bandSettings);
  void updateCursor(VfoId vfoId, const RxPipelineSettings* rxPipelineSettings);
  // void updateCursorA(RxPipelineSettings* rxPipelineSettings);
  // void updateCursorB(RxPipelineSettings* rxPipelineSettings);

  private:
  std::unique_ptr<Ui::StandardFace> ui;

  QtTimeSeriesChart* m_pTimeSeriesChart;
  QtPanadapter* m_pPanadapter;
  QtFrequencyPanel* m_pFrequencyPanel{};
  uint32_t m_reportedIqSampleRate;

  QtSMeter* m_pSmeter;
  float m_rssiMinDbFs;
  float m_rssiMaxDbFs;
};