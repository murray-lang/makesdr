//
// Created by murray on 27/1/26.
//

#include "StandardFace.h"
#include "ui_StandardFace.h"          // generated from StandardFace.ui
// #include "../factory/RegisterFace.h"
#include <ui/qt/widgets/QtChartTheme.h>
#include <ui/qt/widgets/QtPanadapter.h>
#include <ui/qt/widgets/QtTimeSeriesChart.h>
#include <ui/qt/widgets/QtFrequencyPanel.h>
#include <ui/qt/widgets/QtSMeter.h>
#include <ui/qt/faces/RegisterFace.h>
#include <QStyle>



StandardFace::StandardFace(QWidget* parent)
  : FaceBase(parent)
  , ui(std::make_unique<Ui::StandardFace>())
  , m_pTimeSeriesChart(nullptr)
  , m_pPanadapter(nullptr)
  , m_reportedIqSampleRate(0)
  , m_pSmeter(nullptr)
  , m_rssiMinDbFs(-114.0f)
  , m_rssiMaxDbFs(-20.0f)
{
  ui->setupUi(this);
  setAttribute(Qt::WA_StyledBackground, true);
  // qDebug() << "StandardFace objectName:" << this->objectName()
  //          << "class:" << this->metaObject()->className();
}

StandardFace::~StandardFace()
{
  delete m_pPanadapter;
  delete m_pTimeSeriesChart;
}

void
StandardFace::initialise(const RadioSettings* pRadioSettings, IRadioSettingsUpdater* updater)
{
  FaceBase::initialise( pRadioSettings, updater);
  auto* chartTheme = new QtChartTheme(this);
  chartTheme->setObjectName("chartTheme");
  chartTheme->setFixedSize(0, 0); // User can't see it, but Style Engine will style it

  m_pPanadapter = new QtPanadapter(this, "panadapterView", "chartTheme");
  m_pPanadapter->initialise();

  m_pTimeSeriesChart = new QtTimeSeriesChart(this, "timeseriesView", "chartTheme");
  m_pTimeSeriesChart->initialise();

  if (ui->sMeterSlot != nullptr && m_pSmeter == nullptr) {

    if (ui->sMeterSlot->layout() == nullptr) {
      auto* l = new QVBoxLayout(ui->sMeterSlot);
      l->setContentsMargins(0, 0, 0, 0);
      l->setSpacing(0);
    }
    m_pSmeter = new QtSMeter(ui->sMeterSlot);
    m_pSmeter->setObjectName("sMeter");

    m_pSmeter->setReferenceDbFs(-60.0f);
    m_pSmeter->setOrientation(Qt::Horizontal);

    m_pSmeter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // m_pSmeter->style()->polish(m_pSmeter);
    // m_pSmeter->update();
    // m_pSmeter->setMinimumHeight(18);
    // m_pSmeter->setMaximumHeight(24);

    ui->sMeterSlot->layout()->addWidget(m_pSmeter);
  }

  // ui->volumeSlider->setRange(0, 100);
  // ui->volumeSlider->setValue(100);

  // Centered frequency readout in grid row 2
  if (ui->frequencyReadoutSlot != nullptr) {
    auto* slotLayout = new QHBoxLayout(ui->frequencyReadoutSlot);
    slotLayout->setContentsMargins(0, 0, 0, 0);

    m_pFrequencyPanel = new QtFrequencyPanel(m_radioUpdater, ui->frequencyReadoutSlot);
    slotLayout->addWidget(m_pFrequencyPanel);

    m_pFrequencyPanel->initialise(pRadioSettings);
  }
}

// void StandardFace::handleReceiverMeter(const IqReceiverMetering& metering)
// {
//   if (m_pSmeter) {
//     m_pSmeter->setReading(metering.rssiDbFs, metering.agcGainDb);
//   }
// }

void StandardFace::handleRadioSettingsChanged(const RadioSettings* pRadioSettings)
{
  if (pRadioSettings->hasActiveBands()) {
    const ActiveBandSettings* activeBandSettings = pRadioSettings->activeBands();
    if (activeBandSettings->hasFocusBand()) {
      const BandSettings* bandSettings = activeBandSettings->focusBandSettings();
      const BandRfSettings* rfSettings = bandSettings->rfSettings();

      auto centreFrequency = static_cast<int64_t>(rfSettings->frequency());
      if (m_reportedIqSampleRate > 0) {
        int64_t xMin = centreFrequency - (m_reportedIqSampleRate / 2);
        int64_t xMax = centreFrequency + (m_reportedIqSampleRate / 2);
        m_pPanadapter->setSeriesXMinMax(xMin, xMax);
      }
      updatePanadapter(bandSettings);
    }
    m_pFrequencyPanel->applyRadioSettings(pRadioSettings);
  }
}

void
StandardFace::updatePanadapter(const BandSettings* bandSettings)
{
  const RxPipelineSettings* rxPipelineASettings = bandSettings->pipeline(PipelineId::A);
  updateCursor(VfoId::A, rxPipelineASettings);

  if (bandSettings->isMultiPipeline()) {
    const RxPipelineSettings* rxPipelineBSettings = bandSettings->pipeline(PipelineId::B);
    m_pPanadapter->showCursorB(true);
    updateCursor(VfoId::B, rxPipelineBSettings);
  } else {
    m_pPanadapter->showCursorB(false);
  }
}

void
StandardFace::updateCursor(VfoId vfoId, const RxPipelineSettings* rxPipelineSettings)
{
  if (rxPipelineSettings == nullptr) {
    return;
  }
  const PipelineRfSettings& rfSettings = rxPipelineSettings->base().rfSettings();
  int64_t vfo = rfSettings.frequency();
  const Mode& mode = rxPipelineSettings->base().mode();
  if (vfoId == VfoId::A) {
    m_pPanadapter->updateCursorPositionA(vfo, mode.loCut(), mode.hiCut());
  } else {
    m_pPanadapter->updateCursorPositionB(vfo, mode.loCut(), mode.hiCut());
  }
}

// void
// StandardFace::handleReceiverIq(
//   RadioSettings* pRadioSettings,
//   const ComplexSamplesMax* data,
//   uint32_t length,
//   uint32_t sampleRate)
// {
//
//   m_reportedIqSampleRate = sampleRate;
//   RxPipelineSettings* rxPipelineSettings = pRadioSettings->getFocusPipeline();
//   if (rxPipelineSettings != nullptr) {
//     const RfSettings& rfSettings = rxPipelineSettings->getRfSettings();
//     uint32_t centreFrequency = rfSettings.getCentreFrequency();
//     uint32_t xMin = centreFrequency - (sampleRate / 2);
//     uint32_t xMax = centreFrequency + (sampleRate / 2);
//
//     m_pPanadapter->setSeriesXMinMax(xMin, xMax);
//     m_pPanadapter->plot(data, length, sampleRate, centreFrequency, true);
//   }
// }

// void
// StandardFace::handleReceiverAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate)
// {
//   m_pTimeSeriesChart->plot(*data, length);
// }
//
// void
// StandardFace::handleTransmitterIq(
//   RadioSettings* pRadioSettings,
//   const ComplexSamplesMax* data,
//   uint32_t length,
//   uint32_t sampleRate)
// {
//   m_reportedIqSampleRate = sampleRate;
//   TxPipelineSettings* txPipelineSettings = pRadioSettings->getTxPipelineSettings();
//   if (txPipelineSettings != nullptr) {
//     const RfSettings& rfSettings = txPipelineSettings->getRfSettings();
//     uint32_t centreFrequency = rfSettings.getCentreFrequency();
//     uint32_t xMin = centreFrequency - (sampleRate / 2);
//     uint32_t xMax = centreFrequency + (sampleRate / 2);
//
//     m_pPanadapter->setSeriesXMinMax(xMin, xMax);
//     m_pPanadapter->plot(data, length, sampleRate, centreFrequency, true);
//   }
//
//   m_pTimeSeriesChart->plot(*data, length);
// }
//
// void
// StandardFace::handleTransmitterAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate)
// {
//   m_pTimeSeriesChart->plot(*data, length);
// }

REGISTER_FACE(StandardFace, "standard");