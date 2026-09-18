#include <iostream>
#include <sstream>
#include <fstream>

#include "settings/model/core/RadioSettings.h"

#include "settings/model/proto/RadioSettings.pb.h"
#include "settings/model/lookup/radio/generalCoverageRadioLookup.h"

using RadioSettingsPb = makesdr_RadioSettingsPb;

#include <settings/model/proto/ProtobufIo.h>
#include "../../data/exampleRadioSettings.h"

RadioSettingsPb radioSettingsPb = makesdr_RadioSettingsPb_init_zero;
BandSettingsCache bandSettingsCache;


int main()
{
  RadioSettings radioSettings(exampleRadioSettingsPb, generalCoverageRadioLookup, bandSettingsCache);

  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  FieldPath frequencyPath;
  ResultCode rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  FieldUpdate frequencyUpdate(frequencyPath, 1, FieldUpdate::DELTA, isIndirect, trigger);
  rc = radioSettings.applyFieldUpdate(frequencyUpdate);
  if (rc != ResultCode::OK) return -1;

  FieldPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band80 = "80m";
  FieldUpdate band80Update(bandPath, band80, FieldUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applyFieldUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  NameString band40 = "40m";
  FieldUpdate band40Update(bandPath, band40, FieldUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applyFieldUpdate(band40Update);
  if (rc != ResultCode::OK) return -1;

  rc = radioSettings.applyFieldUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  return 0;
}