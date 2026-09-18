#include "QtStepper.h"

#include <ui/qt/widgets/QWidgetPropertySetter.h>

#include "ui_QtStepper.h"

QtStepper::QtStepper(QWidget* parent)
: QWidget(parent)
, ui(std::make_unique<Ui::QtStepper>())
{
  setAttribute(Qt::WA_StyledBackground, true);
  setAttribute(Qt::WA_AcceptTouchEvents, true);

  ui->setupUi(this);

  connect(ui->doubleBackButton, &QToolButton::clicked, this, [this]
  {
    emit step(-2);
  });
  connect(ui->backButton, &QToolButton::clicked, this, [this]
  {
    emit step(-1);
  });
  connect(ui->forwardButton, &QToolButton::clicked, this, [this]
  {
    emit step(1);
  });
  connect(ui->doubleForwardButton, &QToolButton::clicked, this, [this]
  {
    emit step(2);
  });
}

QtStepper::~QtStepper() = default;

void
QtStepper::setFixedProperties()
{
  QWidgetPropertySetter::setWidgetProperty(ui->doubleBackButton, "class", "step doubleBack", false);
  QWidgetPropertySetter::setWidgetProperty(ui->backButton, "class", "step back", false);
  QWidgetPropertySetter::setWidgetProperty(ui->forwardButton, "class", "step forward", false);
  QWidgetPropertySetter::setWidgetProperty(ui->doubleForwardButton, "class", "step doubleForward", false);
}