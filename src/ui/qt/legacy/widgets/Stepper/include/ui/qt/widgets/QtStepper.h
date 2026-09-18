#pragma once

#include <QWidget>
#include <memory>

namespace Ui { class QtStepper; }

class QtStepper : public QWidget
{
  Q_OBJECT
public:
  QtStepper(QWidget *parent = nullptr);
  ~QtStepper() override;

signals:
  void step(int32_t amount);

protected:
  void setFixedProperties();

private:
  std::unique_ptr<Ui::QtStepper> ui;
};