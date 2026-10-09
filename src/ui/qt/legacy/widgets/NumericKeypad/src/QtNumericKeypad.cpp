#include "ui/qt/widgets/QtNumericKeypad.h"
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>

QtNumericKeypad::QtNumericKeypad(const QString& title, int64_t min, int64_t max, QWidget* parent)
  : QDialog(parent)
    , m_intValidator(static_cast<int>(min), static_cast<int>(max), this)
{
  setWindowTitle(title);
  setModal(true); // Blocks interaction with the main window while open

  QVBoxLayout* mainLayout = new QVBoxLayout(this);

  // Display area
  m_display = new QLineEdit(this);
  m_display->setValidator(&m_intValidator);
  m_display->setAlignment(Qt::AlignRight);
  // m_display->setReadOnly(true);
  m_display->setStyleSheet("font-size: 18px; padding: 5px;");
  mainLayout->addWidget(m_display);

  // Grid for buttons
  QGridLayout* gridLayout = new QGridLayout();

  // Define layout structure for standard numeric pad
  const QString labels[4][3] = {
    {"7", "8", "9"},
    {"4", "5", "6"},
    {"1", "2", "3"},
    {"C", "0", "⌫"} // Clear, Zero, Backspace
  };

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 3; ++c) {
      QPushButton* btn = new QPushButton(labels[r][c], this);
      btn->setMinimumSize(60, 60);
      btn->setStyleSheet("font-size: 16px; font-weight: bold;");
      connect(btn, &QPushButton::clicked, this, &QtNumericKeypad::buttonClicked);
      gridLayout->addWidget(btn, r, c);
    }
  }

  mainLayout->addLayout(gridLayout);

  // OK / Close buttons
  QHBoxLayout* actionLayout = new QHBoxLayout();
  QPushButton* btnCancel = new QPushButton("Cancel", this);
  QPushButton* btnOk = new QPushButton("OK", this);

  connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
  connect(btnOk, &QPushButton::clicked, this, &QDialog::accept);

  actionLayout->addWidget(btnCancel);
  actionLayout->addWidget(btnOk);
  mainLayout->addLayout(actionLayout);
}

void QtNumericKeypad::buttonClicked()
{
  QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
  if (!clickedButton) return;

  QString text = clickedButton->text();

  if (text == "C") {
    m_display->clear();
  }
  else if (text == "⌫") {
    m_display->backspace();
  }
  else {
    m_display->insert(text); // Appends the number
  }
}

int64_t QtNumericKeypad::getValue() const
{
  bool ok;
  int value = m_display->text().toInt(&ok);
  if (ok) {
    return static_cast<int64_t>(value);
  }
  return 0;
}

void QtNumericKeypad::setInitialValue(int64_t value)
{
  m_display->setText(QString::number(value));
}
