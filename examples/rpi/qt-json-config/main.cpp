#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <ArduinoJson.h>
#include <ResultCode.h>
#include <config/struct/RadioConfig.h>
#include <config/json/RadioConfig.json.h>
#include <fstream>
#include <settings/control/radio/RadioControl.h>
#include "RadioControlSink.h"

ResultCode loadRadioConfig(const QString& configHome, Config::Radio::Fields& radioConfig)
{
  ResultCode rc = ResultCode::OK;
  // Resolve config path under the current user's home directory: ~/.config/nexusdr/nexusdr.json

  const QString configPath = configHome + "/nexusdr.json";
  if (QFile::exists(configPath)) {
    try {
      std::ifstream f(configPath.toStdString());
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, f, DeserializationOption::NestingLimit(12));
      if (error) {
        qDebug() << "Failed to parse config at" << configPath << ":" << error.c_str();
        rc = ResultCode::ERR_CONFIG_INVALID_JSON;
      } else if (doc["radio"]) {
        // Prefer fromJson for symmetry with toJson()
        rc = Config::Radio::fromJson(doc["radio"].as<JsonVariantConst>(), radioConfig);
      } else {
        qDebug() << "Config file present but no 'radio' section found:" << configPath;
        rc = ResultCode::ERR_CONFIG_MISSING_RADIO;
      }
    } catch (const std::exception& ex) {
      qDebug() << "Failed to read config at" << configPath << ":" << ex.what();
      rc = ResultCode::ERR_CONFIG_FILE_READ_ERROR;
    }
  } else {
    qDebug() << "No config file found at" << configPath << "; using defaults.";
    rc = ResultCode::ERR_CONFIG_FILE_MISSING;
  }
  return rc;
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  const QString configHome = QDir::homePath() + "/.config/nexusdr";
  Config::Radio::Fields radioConfig;
  ResultCode rc = loadRadioConfig(configHome, radioConfig);
  if (rc != ResultCode::OK) {
    return -1;
  }

  RadioControl radioControl;
  rc = radioControl.configure(radioConfig.control);
  if (rc != ResultCode::OK) {
    return -1;
  }
  RadioControlSink radioControlSink;

  radioControl.connectRadioSettingsSink(radioControlSink);
  radioControl.connectSettingFieldUpdateSink(radioControlSink);

  Gpio& gpioInstance = Gpio::getInstance();
  rc = gpioInstance.open();
  if (rc != ResultCode::OK) {
    qDebug() << "Failed to open GPIO interface";
    return -1;
  }
  rc = radioControl.start();
  if (rc != ResultCode::OK) {
    return -1;
  }

  return app.exec();
}
