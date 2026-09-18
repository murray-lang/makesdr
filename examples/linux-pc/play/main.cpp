#include <iostream>
#include <sstream>
#include <fstream>

// #include "settings/model/extra/RadioSettingsEx.h"
// // #include "util/GenerateResolvedPathSourceFiles.h"
//
// #include "settings/model/proto/RadioSettings.pb.h"
// #include <../../../src/settings/model/core/include/settings/model/core/BandSettingsCache.h>
// #include <settings/model/lookup/radio/generalCoverageRadioLookup.h>
// // #include "../settings/util/ResolvedFieldPaths.h"
//
// using RadioSettingsPb = makesdr_RadioSettingsPb;
//
// #include <settings/model/proto/ProtobufIo.h>
// // #include <settings/MessageTagLookup.h>
// #include "../../data/exampleRadioSettings.h"
//
// RadioSettingsPb radioSettingsPb = makesdr_RadioSettingsPb_init_zero;
//
// uint32_t fieldIds[10]; // = { 3, 4, 1, 3 };
// int64_t newFrequency = 7000000;

#include <settings/model/util/GenerateResolvedPathSourceFiles.h>
#include <settings/model/lookup/tag/SplitBandDualIqTagLookup.h>

int main()
{
  generateResolvedPathSourceFiles(split_band_dual_iq_radio_fields, "ResolvedFieldPaths");


  // RadioLookup radioLookup(generalCoverageRadioLookup);
  //
  // RadioSettingsEx radioSettings(exampleRadioSettingsPb, radioLookup);
  // // radioSettings.setBands(&availableBandsPb);
  // // radioSettings.setModes(&availableModesPb);
  //
  // ResultCode rc = radioSettings.autoComplete();
  // if (rc != ResultCode::OK) return -1;
  // // ResultCode rc = setMode(radioSettings, SplitBandId::One, PipelineId::A, Mode::Type::FMN);
  // // ResultCode rc = radioSettings.setFocusMode(Mode::Type::FMN);
  // FieldPath band1BandPath;
  // bool isIndirect = false;
  // AutoCompleteTrigger trigger;
  // rc = RadioSettings::resolveDottedPath("active_bands.band_1.band_request", band1BandPath, &isIndirect, &trigger);
  // if (rc != ResultCode::OK) return -1;
  //
  // NameString band1Name = "160m";
  // FieldUpdate band1RequestUpdate(band1BandPath, band1Name, FieldUpdate::VALUE, isIndirect, trigger);
  //
  // rc = radioSettings.applyUpdate(band1RequestUpdate);
  // if (rc != ResultCode::OK) return -1;
  //
  // FieldPath band2BandPath;
  // rc = RadioSettings::resolveDottedPath("active_bands.band_2.band_request", band2BandPath, &isIndirect, &trigger);
  // if (rc != ResultCode::OK) return -1;
  //
  // NameString band2Name = "30m";
  // FieldUpdate band2RequestUpdate(band2BandPath, band2Name, FieldUpdate::VALUE, isIndirect, trigger);
  //
  // rc = radioSettings.applyUpdate(band2RequestUpdate);
  // if (rc != ResultCode::OK) return -1;
  //
  // FieldPath splitPath;
  // rc = RadioSettings::resolveDottedPath("active_bands.is_split", splitPath, &isIndirect, &trigger);
  // if (rc != ResultCode::OK) return -1;
  //
  // FieldUpdate splitUpdate(splitPath, true, FieldUpdate::VALUE, isIndirect, trigger);
  // rc = radioSettings.applyUpdate(splitUpdate);
  // if (rc != ResultCode::OK) return -1;
  //
  // FieldPath multiPipelinePath;
  // rc = RadioSettings::resolveDottedPath("active_bands.focus_band.is_multi_pipeline", multiPipelinePath, &isIndirect, &trigger);
  // if (rc != ResultCode::OK) return -1;
  //
  // FieldUpdate multiPipelineUpdate(multiPipelinePath, true, FieldUpdate::VALUE, isIndirect, trigger);
  // rc = radioSettings.applyUpdate(multiPipelineUpdate);
  // if (rc != ResultCode::OK) return -1;
  //
  // FieldPath frequencyPath;
  // rc = RadioSettings::resolveDottedPath("active_bands.focus_band.focus_pipeline.base.rf.centre_frequency", frequencyPath, &isIndirect, &trigger);
  // if (rc != ResultCode::OK) return -1;
  //
  // FieldUpdate frequencyUpdate(frequencyPath, 1, FieldUpdate::DELTA, isIndirect, trigger);
  // rc = radioSettings.applyUpdate(frequencyUpdate);
  // if (rc != ResultCode::OK) return -1;
  return 0;
}