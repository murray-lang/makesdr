#pragma once
#include <QAreaSeries>

#include <ui/qt/widgets/QtChartBase.h>
#include <dsp/transforms/fft/Fft.h>

#include <settings/model/radio/iq/FftMessage.h>


class QtPanadapter : public QtChartBase
{
  Q_OBJECT
public:
  QtPanadapter(QWidget* parent, const char* viewName, const char* themeName);
  ~QtPanadapter() override
  {

  };

  void initialise() override;



  void showCursorB(bool show);

  void addPassbandOverlayA(int64_t loCut, int64_t hiCut);
  void addPassbandOverlayB(int64_t loCut, int64_t hiCut);
  void updatePassbandOverlayA(int64_t loCut, int64_t hiCut);
  void updatePassbandOverlayB(int64_t loCut, int64_t hiCut);

  void plot(
    const ComplexSamplesBuffer* timeSeriesData,
    uint32_t length,
    uint32_t sampleRate,
    int64_t centreFrequency,
    bool shuffle = true
  );

  void plot(
    const sdrreal* spectrumData,
    uint32_t length,
    uint32_t sampleRate,
    int64_t centreFrequency,
    bool shuffle = true
  );

  void plot(const FftMessage* fftMsg, int64_t centreFrequency);

  void updateCursorPositionA(int64_t frequency, int32_t loCut, int32_t hiCut);
  void updateCursorPositionB(int64_t frequency, int32_t loCut, int32_t hiCut);

  // Public so the same spectrum can be streamed to other views (e.g. waterfall)
  void powerSpectrum(const ComplexSamplesBuffer& timeSeries, uint32_t timeSeriesLength, RealSamplesBuffer& spectrumOut);

  signals:
  void frequencySelected(int64_t frequency);
  // Horizontal inset of the plot area from the edges of the chart view, in pixels
  void plotAreaMarginsChanged(int left, int right);

protected:
  void handleChartClick(qreal xValue) override;

  void refreshOverlays();
  void emitPlotAreaMargins();

  struct CursorState {
    bool valid = false;
    bool visible = false;
    int64_t frequency = 0;
    int32_t loCut = 0;
    int32_t hiCut = 0;
  };

  float m_floor;
  float m_ceiling;
  Fft<PIPELINE_BUFFER_LENGTH> m_fft;
  CursorState m_cursorA;
  CursorState m_cursorB;

  QAreaSeries m_areaSeries;
  QGraphicsLineItem* m_verticalCursorLineA;
  QGraphicsRectItem* m_filterPassbandRectA;
  QGraphicsLineItem* m_verticalCursorLineB;
  QGraphicsRectItem* m_filterPassbandRectB;
};
