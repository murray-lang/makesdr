#include "MainWindow.h"
#include "ui_mainwindow-1024x600.h"
#include <QSlider>
#include <QThreadPool>
#include <QStyle>
#include <QVariant>
#include <QMenu>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <cmath>

#include <QToolButton>
#include <ui/qt/widgets/QtBandDialog.h>
#include <ui/qt/faces/FaceFactory.h>

#define FFT_SIZE 2048
#define SAMPLE_RATE 192000

constexpr const char * toolbarPopupPropertyName = "isToolbarPopup";

MainWindow::MainWindow(Config::Radio::Fields& radioConfig, QWidget *parent)
  : QMainWindow(parent)
  , m_radioConfig(radioConfig)
  , m_bandUpdateCallback(makeBandUpdateCallback<MainWindow, &MainWindow::bandUpdateCallback>(this))
  , m_radioClient(this)
  , m_bandDialog(nullptr)
  // , m_pRadio(nullptr)
  , ui(new Ui::MainWindow)
  , m_pFaceLayout(new QVBoxLayout)
  , m_reportedIqSampleRate(0)
  ,m_modeButton(nullptr)
  ,m_bandButton(nullptr)
{
  // Initialize Qt resources from icons.qrc
  Q_INIT_RESOURCE(icons);

  m_radioClient.configure(radioConfig);

  connect(&m_radioClient, &QtRadioClient::radioSettingsReceived, this, &MainWindow::handleRadioSettings);
  // connect(&m_radioClient, &QtRadioClient::settingUpdateReceived, this, &MainWindow::handleFieldUpdate);
  // connect(&m_radioClient, &QtRadioClient::receiverAudioReceived, this, &MainWindow::handleReceiverAudio);
  // connect(&m_radioClient, &QtRadioClient::receiverIqReceived, this, &MainWindow::handleReceiverIq);
  // connect(&m_radioClient, &QtRadioClient::meteringReceived, this, &MainWindow::handleReceiverMeter);
  // connect(&m_radioClient, &QtRadioClient::transmitterAudioReceived, this, &MainWindow::handleTransmitterAudio);
  // connect(&m_radioClient, &QtRadioClient::transmitterIqReceived, this, &MainWindow::handleTransmitterIq);

  // initialiseRadio();
  initializeWindow();

  if (this->centralWidget()) {
    // This filter has been added to help manage popup behaviour associated with the toolbar buttons.
    this->installEventFilter(this);
  }
  // connectRadio();
}

MainWindow::~MainWindow()
{
  m_radioClient.stop();
  delete ui;
}

ResultCode
MainWindow::connectRadio()
{
  ResultCode rc = m_radioClient.start();
  if (rc != ResultCode::OK) return rc;

  rc = m_radioClient.requestAll();
  if (rc != ResultCode::OK) return rc;
  // createModeButton(nullptr);
  return ResultCode::OK;
}

// void
// MainWindow::handleReceiverIq(const ComplexSamplesMax* data, uint32_t length, uint32_t sampleRate)
// {
//   if (m_pFace) {
//     m_pFace->handleReceiverIq(&m_radioSettingsCopy, data, length, sampleRate);
//   }
// }
//
// void
// MainWindow::handleReceiverAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate)
// {
//   if (m_pFace) {
//     m_pFace->handleReceiverAudio(data, length, sampleRate);
//   }
// }
//
// void
// MainWindow::handleReceiverMeter(const IqReceiverMetering& metering)
// {
//   if (m_pFace) {
//     m_pFace->handleReceiverMeter(metering);
//   }
// }
//
// void
// MainWindow::handleTransmitterIq(
//   const ComplexSamplesMax* data,
//   uint32_t length,
//   uint32_t sampleRate
//   )
// {
//   if (m_pFace) {
//     m_pFace->handleTransmitterIq(&m_radioSettingsCopy, data, length, sampleRate);
//   }
// }
//
// void
// MainWindow::handleTransmitterAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate)
// {
//   if (m_pFace) {
//     m_pFace->handleTransmitterAudio(data, length, sampleRate);
//   }
// }

void
MainWindow::handleRadioSettings(const RadioSettings* radioSettings /*, uint64_t sequence*/)
{
  if (radioSettings == nullptr) {
    return;
  }
  // uint64_t currentSequence = sequence; //m_pRadio->getUpdateSequenceNo();
  if (radioSettings->purpose() == PayloadPurpose::PURPOSE_REPLACE) {
    handleRefreshedSettings(radioSettings);
  }
  // auto* bandDialog = findChild<QtBandDialog*>("bandPanel");
  if (m_bandDialog != nullptr) {
    const Band* focusBand = radioSettings->getFocusBand();
    if (focusBand != nullptr) {
    m_bandDialog->setSelectedBand(focusBand->name().c_str());

    }
  }
  if (radioSettings->hasActiveBands()) {
    const ActiveBandSettings* activeBandSettings = radioSettings->activeBands();
    if (activeBandSettings->hasFocusBand()) {
      const BandSettings* bandSettings = activeBandSettings->focusBandSettings();
      if (bandSettings != nullptr) {
        updateBandButton(bandSettings->getBand());
        if (bandSettings->hasFocusPipeline()) {
        const RxPipelineSettings* rxPipelineSettings = bandSettings->focusPipeline();
          if (rxPipelineSettings != nullptr) {
            const Mode* mode = &rxPipelineSettings->base().mode();
            updateModeButton(mode);
          }
        }
      }
    }
    const BandSettings* bandSettings = activeBandSettings->focusBandSettings();
    if (bandSettings == nullptr) {
      return;
    }
    const RxPipelineSettings* rxPipelineSettings = bandSettings->focusPipeline();
    if (rxPipelineSettings == nullptr) {
      return;
    }
    const Mode& mode = rxPipelineSettings->base().mode();
  }
  if (m_pFace) {
    m_pFace->handleRadioSettingsChanged(radioSettings);
  }
  // m_radioSettingsCopy.clearChanged();
  // if (sequence == currentSequence) {
  //   bandSettings->clearChanged();
  //   // m_radioSettingsCopy.getBandSelector().clearChanged();
  // }
}

void
MainWindow::handleRefreshedSettings(const RadioSettings* settings)
{
  const Mode* focusMode = settings->getFocusMode();
  addModeMenuToButton(focusMode);
}

void
MainWindow::on_actionBand_triggered()
{
  if (m_bandButton != nullptr) {
    // QWidget* centralWidget = this->centralWidget();
    QWidget* existing = findChild<QWidget*>("bandPanel");
    if (existing) {
      existing->close();
      m_bandDialog = nullptr;
      return;
    }
    const BandCategoryList* bands = m_radioClient.getBands();
    if (bands == nullptr) {
      return;
    }
    const RadioSettings* settings = m_radioClient.getSettings();
    if (settings == nullptr) {
      return;
    }

    const Band* focusBand = settings->getFocusBand();
    m_bandDialog = new QtBandDialog(bands, focusBand->name().c_str(),  m_bandUpdateCallback, this);
    m_bandDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_bandDialog->setObjectName("bandPanel");
    m_bandDialog->setProperty(toolbarPopupPropertyName, true);

    // IMPORTANT: Ensure it has no window flags that would make it a separate window
    m_bandDialog->setWindowFlags(Qt::Widget);

    // We must manually call adjustSize because it's not in a layout
    m_bandDialog->adjustSize();

    QPoint buttonPos = m_bandButton->mapTo(this, QPoint(0, 0));

    int x = buttonPos.x();
    int y = buttonPos.y() - m_bandDialog->height();

    m_bandDialog->move(x, y);
    m_bandDialog->show();
    m_bandDialog->raise();
  }
}

void
MainWindow::handleMainStep(int step)
{
  m_radioClient.stepFocusPipelineFrequency(step);
}

void
MainWindow::closeActiveToolbarPopups()
{
  const auto panels = this->centralWidget()->findChildren<QWidget*>();
  for (auto* panel : panels) {
    if (panel->property(toolbarPopupPropertyName).toBool()) {
      panel->close();
    }
  }
}

bool
MainWindow::eventFilter(QObject *watched, QEvent *event)
{
  if (watched == this && event->type() == QEvent::MouseButtonPress) {
    auto* mouseEvent = static_cast<QMouseEvent*>(event);

    const auto panels = this->findChildren<QWidget*>();
    for (auto* panel : panels) {
      if (panel->property(toolbarPopupPropertyName).toBool() && panel->isVisible()) {
        // If the click is outside this panel, close it
        if (!panel->geometry().contains(mouseEvent->pos())) {
          panel->close();
          return true; // Consume the event so it doesn't click the UI behind
        }
      }
    }
  }
  return QMainWindow::eventFilter(watched, event);
}

// void
// MainWindow::on_actionMode_triggered()
// {
//   qDebug() << "on_actionMode_triggered()";
// }

// void
// MainWindow::on_actionLevels_triggered()
// {
//   qDebug() << "on_actionLevels_triggered()";
// }

void MainWindow::initializeWindow()
{
  ui->setupUi(this);

  m_pFaceLayout = new QVBoxLayout(this->centralWidget());
  m_pFaceLayout->setObjectName("faceLayout");
  m_pFaceLayout->setContentsMargins(0, 0, 0, 0);
  m_pFaceLayout->setSpacing(0);

  if (m_radioConfig.ui && !m_radioConfig.ui->face.empty()) {
    setFaceByName(m_radioConfig.ui->face);
  } else {
    setFaceByName(FaceFactory::defaultName);
  }


  // qDebug() << "Current Icon Theme:" << QIcon::themeName();
  // qDebug() << "Icon Search Paths:" << QIcon::themeSearchPaths();

  addConfigButton();
  addBandButton();

  QWidget* spacer1 = new QWidget();
  spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  ui->toolBar->addWidget(spacer1);

  addStepper();

  QWidget* spacer2 = new QWidget();
  spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  ui->toolBar->addWidget(spacer2);

  addModeButton();
  addLevelsButton();
}

void
MainWindow::setFaceByName(const Config::Ui::FaceString& faceName)
{
  const Config::Ui::FaceString name = faceName.empty() ? FaceFactory::defaultName : faceName;

  if (m_pFace) {
    m_pFaceLayout->removeWidget(m_pFace.get());
    m_pFace.reset();
  }

  m_pFace = FaceFactory::instance().create(name, this->centralWidget());
  if (!m_pFace) {
    // last-resort: keep current face, or show an error widget
    return;
  }

  // m_pFace = move(newFace);
  m_pFaceLayout->addWidget(m_pFace.get());
  m_pFace->initialise(m_radioClient.getSettings(), &m_radioClient);
}

void
MainWindow::addModeButton()
{
  m_modeButton = new QToolButton();
  m_modeButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  m_modeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  // Force the button to draw its background based on the current palette
  m_modeButton->setAutoFillBackground(true);
  m_modeButton->setProperty("class", "toolbarButton toolbarButtonC");
  updateModeButton(nullptr);
  ui->toolBar->addWidget(m_modeButton);
}

void
MainWindow::addModeMenuToButton(const Mode* selectedMode)
{
  const ModeList* modes = m_radioClient.getModes();
  if (modes == nullptr) {
    return;
  }

  QMenu* modeMenu = createModeMenu(modes, selectedMode);
  updateModeButton(selectedMode);
  modeMenu->setProperty("class", "toolbarMenu mode");
  connect(m_modeButton, &QToolButton::pressed, this, [this, modeMenu, selectedMode]() {
    // Calculate the top-left position of the button in global screen coordinates
    QPoint pos = m_modeButton->mapToGlobal(QPoint(0, 0));

    // Move the point up by the height of the menu
    // hint: sizeHint() is usually accurate for menus before they are shown
    pos.setY(pos.y() - modeMenu->sizeHint().height());

    for (QAction *action : modeMenu->actions()) {
      bool checked = selectedMode != nullptr && action->text() == selectedMode->name().c_str();
      action->setChecked(checked);
    }
    modeMenu->exec(pos);
  });
}

QMenu*
MainWindow::createModeMenu(const ModeList* modes, const Mode* selectedMode)
{
  auto modeMenu = new QMenu(this);

  auto* actionGroup = new QActionGroup(this);
  actionGroup->setExclusive(true); // Only one can be checked at a time


  for (const auto& modePb : modes->modes()) {
    Mode mode(modePb);
    // Mode::Type modeType = static_cast<Mode::Type>(mode.type);
    QAction* action = modeMenu->addAction(mode.name().c_str(), this, [this, mode]()
    {

      RadioSettingsUpdater* updater = m_radioClient.getUpdater();
      if (updater != nullptr) {
        updater->setFocusMode(mode.type());
      }
    });
    action->setCheckable(true);
    action->setActionGroup(actionGroup);

    // Highlight the currently active mode
    if (selectedMode != nullptr && mode.type() == selectedMode->type()) {
      action->setChecked(true);
    }
  }
  return modeMenu;
}

void
MainWindow::updateModeButton(const Mode* mode)
{
  if (m_modeButton != nullptr) {
    if (mode == nullptr) {
      m_modeButton->setText("Mode");
    } else {
      m_modeButton->setText(mode->name().c_str());
    }
  }
}

void
MainWindow::updateBandButton(const Band* band)
{
  if (m_bandButton != nullptr && band != nullptr) {
    m_bandButton->setText(QString::fromStdString(band->label().c_str()));
  }
}

void
MainWindow::bandUpdateCallback(const char * bandName)
{
  RadioSettingsUpdater* shortcuts = m_radioClient.getUpdater();
  if (shortcuts != nullptr) {
    shortcuts->selectBand(bandName);
  }
}

// void
// MainWindow::updateModeMenu(const Mode& mode)
// {
//   if (m_modeButton != nullptr) {
//     m_modeButton->setText(mode.getName().c_str());
//   }
// }

void
MainWindow::addLevelsButton()
{
  auto* levelsBtn = new QToolButton();
  // levelsBtn->setDefaultAction(ui->actionLevels);
  // tabsBtn->setFixedWidth(100);
  levelsBtn->setIcon(QIcon(":ui//icons/solid/sliders.svg"));
  levelsBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  levelsBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  // Force the button to draw its background based on the current palette
  levelsBtn->setAutoFillBackground(true);
  levelsBtn->setProperty("class", "toolbarButton toolbarButtonD");
  ui->toolBar->addWidget(levelsBtn);
}

void
MainWindow::addConfigButton()
{
  auto* configBtn = new QToolButton();
  // configBtn->setDefaultAction(ui->actionConfigure);
  // tabsBtn->setFixedWidth(100);
  configBtn->setIcon(QIcon(":ui//icons/solid/gear.svg"));
  configBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  // Force the button to draw its background based on the current palette
  configBtn->setAutoFillBackground(true);
  configBtn->setProperty("class", "toolbarButton toolbarButtonA");
  ui->toolBar->addWidget(configBtn);
}

void
MainWindow::addBandButton()
{
  m_bandButton = new QToolButton();
  m_bandButton->setDefaultAction(ui->actionBand);
  m_bandButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  m_bandButton->setAutoFillBackground(true);
  m_bandButton->setProperty("class", "toolbarButton toolbarButtonB");
  ui->toolBar->addWidget(m_bandButton);
}

void
MainWindow::addStepper()
{
  m_stepper = new QtStepper();
  m_stepper->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  m_stepper->setAutoFillBackground(true);
  m_stepper->setProperty("class", "toolbarButton mainStepper");
  ui->toolBar->addWidget(m_stepper);

  connect(m_stepper, &QtStepper::step, this, &MainWindow::handleMainStep);
}
