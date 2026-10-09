#pragma once

#include <QImage>
#include <QWidget>

#include <samples/SampleTypes.h>
#include <ui/common/waterfall/Waterfall.h>

#include <settings/model/radio/iq/FftMessage.h>

// Qt host for the platform-neutral Waterfall. All the spectrum-to-pixel work
// happens in Waterfall<Argb8888>; this class only owns the QImage it writes
// into, blits it, and translates styling and mouse clicks.
class QtWaterfall : public QWidget
{
  Q_OBJECT
  // Comma-separated colors spaced evenly from weakest to strongest signal,
  // e.g. "#000000, #00008c, #00a0ff, #00dc3c, #ffe600, #ff2800, #ffffff"
  Q_PROPERTY(QString paletteColors READ paletteColors WRITE setPaletteColors)
  Q_PROPERTY(qreal minDb READ minDb WRITE setMinDb)
  Q_PROPERTY(qreal maxDb READ maxDb WRITE setMaxDb)

public:
  explicit QtWaterfall(QWidget* parent = nullptr);
  ~QtWaterfall() override = default;

  void addSpectrum(
    const RealSamplesBuffer* spectrumData,
    uint32_t sampleRate,
    int64_t centreFrequency,
    bool shuffle = true
  );

  void addSpectrum(const FftMessage* fftMsg, int64_t centreFrequency);

  void addSpectrum(
    const uint8_t* bins,
    uint32_t numBins,
    uint32_t sampleRate,
    int64_t centreFrequency,
    bool shuffle = true
  );

  void clear();

  // Inset the image horizontally so its columns line up with another chart's
  // plot area (e.g. the panadapter's, which is offset by its y-axis labels)
  void setPlotMargins(int left, int right);

  [[nodiscard]] QString paletteColors() const { return m_paletteColors; }
  void setPaletteColors(const QString& colors);

  [[nodiscard]] qreal minDb() const { return m_waterfall.minDb(); }
  void setMinDb(qreal db) { m_waterfall.setDbRange(static_cast<float>(db), m_waterfall.maxDb()); }

  [[nodiscard]] qreal maxDb() const { return m_waterfall.maxDb(); }
  void setMaxDb(qreal db) { m_waterfall.setDbRange(m_waterfall.minDb(), static_cast<float>(db)); }

signals:
  void frequencySelected(int64_t frequency);

protected:
  void paintEvent(QPaintEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

private:
  [[nodiscard]] QRect imageRect() const;
  void reallocate();

  Waterfall<Argb8888> m_waterfall;
  QImage m_image;
  QString m_paletteColors;
  int m_leftMargin;
  int m_rightMargin;
};
