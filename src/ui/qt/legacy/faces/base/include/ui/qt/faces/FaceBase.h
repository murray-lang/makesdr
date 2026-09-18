#pragma once
#include <QWidget>

#include <samples/SampleTypes.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>
#include <settings/model/radios/IRadioSettingsUpdater.h>


using RadioSettings = SplitBandDualIqRxTxSettings;

class Radio;

class FaceBase : public QWidget //, public FieldUpdateSource //, public FieldUpdateSink
{
  Q_OBJECT
public:
  using QWidget::QWidget;
  FaceBase(QWidget* pParent)
    : QWidget(pParent)
    , m_radioUpdater(nullptr)
  {}
  ~FaceBase() override = default;

  virtual void initialise(const RadioSettings* pRadioSettings, IRadioSettingsUpdater* updater)
  {
    // m_pRadioSettings = pRadioSettings;
    m_radioUpdater = updater;
  }

  virtual void handleRadioSettingsChanged(const RadioSettings* pRadioSettings) {}


  void setNames(const std::string& name, const std::string& label) { m_name = name; m_label = label; }
  [[nodiscard]] const std::string& getName() const { return m_name; }
  [[nodiscard]] const std::string& getLabel() const { return m_label; }
  // virtual void setRadio(Radio* pRadio) { m_pRadio = pRadio; }
  // [[nodiscard]] Radio* getRadio() const { return m_pRadio; }

  // virtual void handleReceiverIq(
  //   RadioSettings* pRadioSettings,
  //   const ComplexSamplesMax* data,
  //   uint32_t length,
  //   uint32_t sampleRate) = 0;

  // virtual void handleReceiverAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate) = 0;
  // virtual void handleReceiverMeter(const IqReceiverMetering& metering) = 0;

  // virtual void handleTransmitterIq(
  //   RadioSettings* pRadioSettings,
  //   const ComplexSamplesMax* data,
  //   uint32_t length,
  //   uint32_t sampleRate) = 0;
  // virtual void handleTransmitterAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate) = 0;


protected:
  std::string m_name;
  std::string m_label;
  IRadioSettingsUpdater* m_radioUpdater;
  // RadioSettings* m_pRadioSettings;
  // Radio* m_pRadio;
  // FieldUpdateSink* m_pFieldUpdateSink;
};
