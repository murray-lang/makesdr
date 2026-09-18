#pragma once
#include <ui/qt/widgets/QtChartBase.h>


class QtTimeSeriesChart : public QtChartBase
{
public:
  QtTimeSeriesChart(QWidget* parent, const char* viewName, const char* themeName);
  ~QtTimeSeriesChart() override = default;

  void initialise() override;

};
