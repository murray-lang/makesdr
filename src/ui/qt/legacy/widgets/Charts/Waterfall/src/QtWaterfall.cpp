#include "ui/qt/widgets/QtWaterfall.h"

#include <QMouseEvent>
#include <QPainter>

#include <algorithm>
#include <vector>

QtWaterfall::QtWaterfall(QWidget* parent)
  : QWidget(parent)
  , m_leftMargin(0)
  , m_rightMargin(0)
{
  // Margins are left unpainted so the parent's background shows through
}

void
QtWaterfall::addSpectrum(
    const RealSamplesBuffer* spectrumData,
    uint32_t sampleRate,
    int64_t centreFrequency,
    bool shuffle)
{
  if (spectrumData == nullptr) {
    return;
  }
  m_waterfall.addSpectrum<sdrreal>({
    spectrumData->data(),
    spectrumData->size(),
    centreFrequency,
    sampleRate,
    shuffle
  });
  update(imageRect());
}

void
QtWaterfall::addSpectrum(const FftMessage* fftMsg, int64_t centreFrequency)
{
  m_waterfall.addSpectrum(fftMsg, centreFrequency);
  update(imageRect());
}

void
QtWaterfall::addSpectrum(
    const uint8_t* bins,
    uint32_t numBins,
    uint32_t sampleRate,
    int64_t centreFrequency,
    bool shuffle
  )
{
  if (bins == nullptr) {
    return;
  }
  m_waterfall.addSpectrum<uint8_t>({
    bins,
    numBins,
    centreFrequency,
    sampleRate,
    shuffle
  });
  update(imageRect());
}

void
QtWaterfall::clear()
{
  m_waterfall.clear();
  update();
}

void
QtWaterfall::setPlotMargins(int left, int right)
{
  left = std::max(0, left);
  right = std::max(0, right);
  if (left != m_leftMargin || right != m_rightMargin) {
    m_leftMargin = left;
    m_rightMargin = right;
    reallocate();
    update();
  }
}

void
QtWaterfall::setPaletteColors(const QString& colors)
{
  m_paletteColors = colors;

  std::vector<ColorStop> stops;
  const QStringList names = colors.split(',', Qt::SkipEmptyParts);
  for (const QString& name : names) {
    QColor c(name.trimmed());
    if (c.isValid()) {
      stops.push_back({ 0.0f, { static_cast<uint8_t>(c.red()), static_cast<uint8_t>(c.green()), static_cast<uint8_t>(c.blue()) } });
    }
  }
  if (stops.size() < 2) {
    m_waterfall.colorMap().build(ColorMaps::Classic);
  } else {
    for (size_t i = 0; i < stops.size(); i++) {
      stops[i].position = static_cast<float>(i) / static_cast<float>(stops.size() - 1);
    }
    m_waterfall.colorMap().build(stops.data(), stops.size());
  }
  // Existing rows were colored with the old map
  m_waterfall.clear();
  update();
}

QRect
QtWaterfall::imageRect() const
{
  QRect r = rect();
  r.setLeft(r.left() + m_leftMargin);
  r.setRight(r.right() - m_rightMargin);
  return r;
}

void
QtWaterfall::reallocate()
{
  QRect r = imageRect();
  if (r.width() <= 0 || r.height() <= 0) {
    m_waterfall.detach();
    m_image = QImage();
    return;
  }
  if (m_image.size() == r.size()) {
    return;
  }
  m_image = QImage(r.size(), QImage::Format_RGB32);
  m_waterfall.attach(
    reinterpret_cast<uint32_t*>(m_image.bits()),
    static_cast<uint16_t>(m_image.width()),
    static_cast<uint16_t>(m_image.height()),
    static_cast<size_t>(m_image.bytesPerLine())
  );
}

void
QtWaterfall::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  reallocate();
}

void
QtWaterfall::paintEvent(QPaintEvent* /*event*/)
{
  if (m_image.isNull()) {
    return;
  }
  QPainter painter(this);
  QRect target = imageRect();

  Waterfall<Argb8888>::Segment segments[2];
  size_t count = m_waterfall.segments(segments);
  for (size_t i = 0; i < count; i++) {
    const auto& s = segments[i];
    QRect source(0, s.sourceRow, m_image.width(), s.rows);
    QRect dest(target.left(), target.top() + s.displayRow, m_image.width(), s.rows);
    painter.drawImage(dest, m_image, source);
  }
}

void
QtWaterfall::mousePressEvent(QMouseEvent* event)
{
  if (event->button() == Qt::LeftButton) {
    QRect r = imageRect();
    qreal x = event->position().x() - r.left();
    if (x >= 0 && x < r.width()) {
      double frequency = m_waterfall.frequencyAt(x);
      if (frequency > 0.0) {
        emit frequencySelected(static_cast<int64_t>(frequency));
      }
    }
  }
  QWidget::mousePressEvent(event);
}
