#pragma once

#include <QMainWindow>
#include <ui/qt/QtRadioClient.h>
#include <ui/qt/faces/FaceBase.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>
#include <ui/qt/widgets/QtBandDialog.h>
#include <ui/qt/widgets/QtStepper.h>

class QVBoxLayout;
class QtTimeSeriesChart;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QToolButton;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  using RadioSettings = SplitBandDualIqRxTxSettings;
  using ActiveBandSettings = RadioSettings::ActiveBandSettings;
  using BandSettings = ActiveBandSettings::BandSettings;

  explicit MainWindow(Config::Radio::Fields& radioConfig, QWidget *parent = nullptr);
  ~MainWindow() override;

  ResultCode connectRadio();

  // void customEvent(QEvent* event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
  // void on_actionConfigure_triggered();
  void on_actionBand_triggered();
  void handleMainStep(int step);
  // void on_actionMode_triggered();
  // void on_actionLevels_triggered();
  void handleRadioSettings(const RadioSettings* settings/*, uint64_t sequence*/);
  // void handleReceiverIq(const ComplexSamplesMax* data, uint32_t length, uint32_t sampleRate ) ;
  // void handleReceiverAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate);
  // // void handleReceiverMeter(const IqReceiverMetering& metering);
  // void handleTransmitterIq( const ComplexSamplesMax* data, uint32_t length, uint32_t sampleRate );
  // void handleTransmitterAudio(const RealSamplesMax* data, uint32_t length, uint32_t sampleRate);

protected:


  void closeActiveToolbarPopups();

  void handleRefreshedSettings(const RadioSettings* settings);

private:
  void initializeWindow();

  void setFaceByName(const Config::Ui::FaceString& faceName);

  void addModeButton();
  void addModeMenuToButton(const Mode* selectedMode);
  QMenu* createModeMenu(const ModeList* modes, const Mode* currentMode);
  void updateModeButton(const Mode* mode);
  void updateBandButton(const Band* band);
  // void updateModeMenu(const Mode& mode);
  void addLevelsButton();
  void addConfigButton();
  void addStepper();
  void addBandButton();
  void bandUpdateCallback(const char * bandName);

private:
  Config::Radio::Fields& m_radioConfig;
  Ui::MainWindow *ui;

  QtRadioClient m_radioClient;
  QtBandDialog* m_bandDialog;
  BandUpdateCallback m_bandUpdateCallback;

  unique_ptr<FaceBase> m_pFace;
  QVBoxLayout* m_pFaceLayout;

  // Radio* m_pRadio;

  uint32_t m_reportedIqSampleRate;
  // RadioSettings m_radioSettingsCopy;

  QToolButton* m_modeButton;
  QToolButton* m_bandButton;
  QtStepper* m_stepper;
};

