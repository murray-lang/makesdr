#include <iostream>
#include <sstream>
#include <fstream>

#include "settings/radio/RadioSettings.h"
#include "util/GenerateResolvedPathSourceFiles.h"

#include "RadioSettings.pb.h"
#include "settings/radio/writeable/WriteableRadioSettings.h"
#include "settings/band/writeable/AvailableBands.h"
#include "settings/band/writeable/BandSettingsCache.h"
#include "mode/AvailableModes.h"
#include "radio/ReadOnlyRadioSettings.h"
#include "settings/util/ResolvedFieldPaths.h"

using RadioSettingsPb = RadioSettings_RadioSettingsPb;

#include "util/PbUtils.h"
#include "util/MessageTagLookup.h"

RadioSettingsPb radioSettingsPb = RadioSettings_RadioSettingsPb_init_zero;

uint32_t fieldIds[10]; // = { 3, 4, 1, 3 };
int64_t newFrequency = 7000000;

int main()
{
  generateResolvedPathSourceFiles(active_bands_fields, "ResolvedActiveBandsFieldPaths");

  BandCategoryList bandInfo(availableBandsPb);
  ModeList modeInfo(availableModesPb);

  ReadOnlyRadioSettings basicRadioSettings;

  WriteableRadioSettings radioSettings(bandInfo, modeInfo);
  // radioSettings.setBands(&availableBandsPb);
  // radioSettings.setModes(&availableModesPb);


  // ResultCode rc = setMode(radioSettings, SplitBandId::One, PipelineId::A, Mode::Type::FMN);
  // ResultCode rc = radioSettings.setFocusMode(Mode::Type::FMN);
  SettingFieldPath band1BandPath;
  bool isIndirect = false;
  AutoCompleteTrigger trigger;
  ResultCode rc = WriteableRadioSettings::resolveDottedPath("active_bands.band_1.band_request", band1BandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band1Name = "160m";
  SettingFieldUpdate band1RequestUpdate(band1BandPath, band1Name, isIndirect, trigger);

  rc = radioSettings.applyUpdate(band1RequestUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingFieldPath band2BandPath;
  rc = WriteableRadioSettings::resolveDottedPath("active_bands.band_2.band_request", band2BandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band2Name = "30m";
  SettingFieldUpdate band2RequestUpdate(band2BandPath, band2Name, isIndirect, trigger);

  rc = radioSettings.applyUpdate(band2RequestUpdate);
  if (rc != ResultCode::OK) return -1;

  SettingFieldPath splitPath;
  rc = WriteableRadioSettings::resolveDottedPath("active_bands.is_split", splitPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  SettingFieldUpdate splitUpdate(splitPath, true, isIndirect, trigger);
  rc = radioSettings.applyUpdate(splitUpdate);
  if (rc != ResultCode::OK) return -1;
  return 0;
}