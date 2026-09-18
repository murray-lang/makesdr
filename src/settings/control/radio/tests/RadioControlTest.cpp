#include <gtest/gtest.h>
#include "ExposedRadioControl.h"
#include <ArduinoJson.h>
#include <config/json/ControlConfig.json.h>
// #include <sstream>
#include <fstream>
#include <QDir>

#include "linux-pc-RadioControlConfig.h"



// Basic instantiation test
TEST(RadioControlTest, CanInstantiate) {
  RadioControl radioControl;
  EXPECT_TRUE(true); // Basic smoke test
}

// Test that we can create and destroy without crashing
TEST(RadioControlTest, ConstructorDestructor) {
  auto* transport = new RadioControl();
  EXPECT_NE(transport, nullptr);
  delete transport;
}

TEST(RadioControlTest, CanLoadConfig) {
  ResultCode rc = ResultCode::OK;
  JsonDocument jsonDoc;
  DeserializationError error = deserializeJson(jsonDoc, radioControlConfigJson, DeserializationOption::NestingLimit(12));
  if (!error) {
    Config::Control::Fields controlConfig;
    rc = Config::Control::fromJson(jsonDoc, controlConfig);
    if (rc == ResultCode::OK) {
      SplitBandDualIqRxTxSettings settings;
      RadioControl radioControl;
      rc = radioControl.configure(controlConfig, settings.resolveDottedStringFunc());
    }
  } else {
    rc = ResultCode::ERR_CONFIG_INVALID_JSON;
  }

  EXPECT_EQ(rc, ResultCode::OK);
}