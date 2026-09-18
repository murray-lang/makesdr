#include <radios/iq/SplitBandDualIqRadio.h>
#include <settings/model/radios/iq/SplitBandDualIqRxTxSettings.h>
#include <settings/model/data/band/mostBandCategories.h>
#include <settings/model/data/mode/basicModes.h>
#include <config/json/RadioConfig.json.h>
#include <linux/test-utils/QtTransportTestRadio.h>
#include <radios/iq/SplitBandDualIqRadio.h>
#include <fstream>

// #include <QCoreApplication>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <ui/qt/util/QtUtil.h>
#include <ui/qt/MainWindow.h>
#include <test-utils/testRadioSettings.h>

// #include <settings/model/path/generateResolvedPathSourceFiles.h>
// #include <settings/model/path/SplitBandDualIqTagLookup.h>

BandCategoryList bandsByCategory(mostBandCategories);
ModeList modeList(basicModes);
SplitBandDualIqRxTxSettings::Cache bandSettingsCache;

int main(int argc, char *argv[])
{
  // generateResolvedPathSourceFiles(split_band_dual_iq_radio_fields, "SplitBandDualIqResolved");
  // return 0;
  // QtTransportTestRadio radio;
  // SplitBandDualIqRadio radio(bandsByCategory, modeList, bandSettingsCache);

  // ResultCode rc = radio.configure();
  // if (rc != ResultCode::OK) {
  //   return -1;
  // }
  // QCoreApplication app(argc, argv);

  const QString configHome = QDir::homePath() + "/.config/nexusdr";
  const QString configPath = configHome + "/dual-iq-split-radio-test.json";
  Config::Radio::Fields radioConfig;
  ResultCode rc = loadRadioConfig(configPath, radioConfig);
  if (rc != ResultCode::OK) {
    return -1;
  }
  SplitBandDualIqRadio radio(bandsByCategory, modeList, bandSettingsCache);
  rc = radio.configure(radioConfig);
  if (rc != ResultCode::OK) {
    return -1;
  }
  SplitBandDualIqRxTxSettings settings(testRadioSettingsPayloadPb);
  radio.setSettings(settings);

  QApplication app(argc, argv);

  rc = loadStylesheets(configHome, app);
  if (rc != ResultCode::OK) {
    qDebug() << "Error loading stylesheets: " << static_cast<uint32_t>(rc);
    return -1;
  }

  rc = radio.start();
  if (rc != ResultCode::OK) {
    radio.stop();
    qDebug() << "Error starting radio: " << static_cast<uint32_t>(rc);
    return 1;
  }

  MainWindow w(radioConfig);
  rc = w.connectRadio();
  if (rc != ResultCode::OK) {
    qDebug() << "Error connecting to radio: " << static_cast<uint32_t>(rc);
    return 1;
  }

  w.show();

  int rc2 = app.exec();

  radio.stop();

  // while (true) {
  //   // QCoreApplication::processEvents();
  //   std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // }

  return 0; //rc2;
}