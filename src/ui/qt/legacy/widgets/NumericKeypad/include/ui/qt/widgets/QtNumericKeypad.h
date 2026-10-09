#pragma once

#include <QDialog>
#include <QLineEdit>
#include <qvalidator.h>

class QtNumericKeypad : public QDialog {
  Q_OBJECT

public:
  explicit QtNumericKeypad(const QString& title, int64_t min, int64_t max, QWidget *parent = nullptr);
  int64_t getValue() const;
  void setInitialValue(int64_t value);

private slots:
    void buttonClicked();

private:
  QLineEdit *m_display;
  QIntValidator m_intValidator;

};
