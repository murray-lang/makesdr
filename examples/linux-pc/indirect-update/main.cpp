#include <iostream>
#include <sstream>
#include <fstream>

#include "settings/model/core/RadioSettings.h"
// #include "util/GenerateResolvedPathSourceFiles.h"

#include "settings/model/proto/RadioSettings.pb.h"
#include "settings/model/meta/generalCoverageRadioMeta.h"

using RadioSettingsPb = RadioSettings_RadioSettingsPb;

#include <settings/model/core/ProtobufIo.h>
// #include <settings/MessageTagLookup.h>
#include "../../data/exampleRadioSettings.h"

RadioSettingsPb radioSettingsPb = RadioSettings_RadioSettingsPb_init_zero;

uint32_t fieldIds[10]; // = { 3, 4, 1, 3 };
int64_t newFrequency = 7000000;

int main()
{
  RadioSettings radioSettings(exampleRadioSettingsPb, generalCoverageRadioMeta);

  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  SettingFieldPath frequencyPath;
  ResultCode rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  SettingFieldUpdate frequencyUpdate(frequencyPath, 1, SettingFieldUpdate::DELTA, isIndirect, trigger);
  rc = radioSettings.applySettingFieldUpdate(frequencyUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingFieldPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString bandName = "80m";
  SettingFieldUpdate bandUpdate(bandPath, bandName, SettingFieldUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applySettingFieldUpdate(bandUpdate);
  if (rc != ResultCode::OK) return -1;

  return 0;
}