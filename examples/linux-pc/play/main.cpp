#include <iostream>
#include <sstream>
#include <fstream>

#include "settings/model/extra/RadioSettingsEx.h"
// #include "util/GenerateResolvedPathSourceFiles.h"

#include "settings/model/proto/RadioSettings.pb.h"
#include <../../../src/settings/model/core/include/settings/model/core/BandSettingsCache.h>
#include <settings/model/meta/generalCoverageRadioMeta.h>
// #include "../settings/util/ResolvedFieldPaths.h"

using RadioSettingsPb = makesdr_RadioSettingsPb;

#include <settings/model/proto/ProtobufIo.h>
// #include <settings/MessageTagLookup.h>
#include "../../data/exampleRadioSettings.h"

RadioSettingsPb radioSettingsPb = makesdr_RadioSettingsPb_init_zero;

uint32_t fieldIds[10]; // = { 3, 4, 1, 3 };
int64_t newFrequency = 7000000;

int main()
{
  // generateResolvedPathSourceFiles(active_bands_fields, "ResolvedActiveBandsFieldPaths");


  RadioMeta radioMeta(generalCoverageRadioMeta);

  RadioSettingsEx radioSettings(exampleRadioSettingsPb, radioMeta);
  // radioSettings.setBands(&availableBandsPb);
  // radioSettings.setModes(&availableModesPb);

  ResultCode rc = radioSettings.autoComplete();
  if (rc != ResultCode::OK) return -1;
  // ResultCode rc = setMode(radioSettings, SplitBandId::One, PipelineId::A, Mode::Type::FMN);
  // ResultCode rc = radioSettings.setFocusMode(Mode::Type::FMN);
  SettingPath band1BandPath;
  bool isIndirect = false;
  AutoCompleteTrigger trigger;
  rc = RadioSettings::resolveDottedPath("active_bands.band_1.band_request", band1BandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band1Name = "160m";
  SettingUpdate band1RequestUpdate(band1BandPath, band1Name, SettingUpdate::VALUE, isIndirect, trigger);

  rc = radioSettings.applyUpdate(band1RequestUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingPath band2BandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.band_2.band_request", band2BandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band2Name = "30m";
  SettingUpdate band2RequestUpdate(band2BandPath, band2Name, SettingUpdate::VALUE, isIndirect, trigger);

  rc = radioSettings.applyUpdate(band2RequestUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingPath splitPath;
  rc = RadioSettings::resolveDottedPath("active_bands.is_split", splitPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  SettingUpdate splitUpdate(splitPath, true, SettingUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applyUpdate(splitUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingPath multiPipelinePath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.is_multi_pipeline", multiPipelinePath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  SettingUpdate multiPipelineUpdate(multiPipelinePath, true, SettingUpdate::VALUE, isIndirect, trigger);
  rc = radioSettings.applyUpdate(multiPipelineUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingPath frequencyPath;
  rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  SettingUpdate frequencyUpdate(frequencyPath, 1, SettingUpdate::DELTA, isIndirect, trigger);
  rc = radioSettings.applyUpdate(frequencyUpdate);
  if (rc != ResultCode::OK) return -1;
  return 0;
}