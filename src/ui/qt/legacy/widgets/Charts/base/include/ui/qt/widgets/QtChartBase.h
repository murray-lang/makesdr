#pragma once

#include <QChartView>
#include <QLineSeries>
#include <QWidget>

#include "QtChartTheme.h"
#include <samples/SampleTypes.h>


class QtChartBase : public QWidget
{
  Q_OBJECT
public:
  QtChartBase(QWidget* parent, const char* viewName, const char* themeName);
  ~QtChartBase() override = default;

  virtual void initialise();

  virtual void plot(const RealSamplesBuffer& data, uint32_t length);
  virtual void plot(const ComplexSamplesBuffer& data, uint32_t length);

  void setSeriesXMinMax(int64_t min, int64_t max);



  bool eventFilter(QObject *watched, QEvent *event) override;

protected:
  virtual void handleChartClick(qreal xValue) {}

  void handleChartClick(const QPointF &scenePos);

  virtual void applyTheme();


  QWidget* m_pParent;
  QString m_viewName;
  QString m_themeName;
  QChartView* m_pChartView;
  QChart* m_pChart;
  QtChartTheme* m_pTheme;
  QLineSeries m_lineSeries;
  int64_t m_xMin;
  int64_t m_xMax;
};
