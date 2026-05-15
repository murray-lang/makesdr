#include <iostream>
#include <sstream>
#include <fstream>

#include "RadioSettings.h"
#include "util/GenerateResolvedPathSourceFiles.h"

#include "RadioSettings.pb.h"
#include "settings/util/ResolvedFieldPaths.h"

using RadioSettingsPb = RadioSettings_RadioSettingsPb;

#include "util/PbUtils.h"
#include "util/MessageTagLookup.h"

RadioSettingsPb radioSettingsPb = RadioSettings_RadioSettingsPb_init_zero;

uint32_t fieldIds[10]; // = { 3, 4, 1, 3 };
int64_t newFrequency = 7000000;

int main()
{
  generateResolvedPathSourceFiles();

  // radioSettings.active_bands.band_1.band.lowest_frequency = 14000000;
  // std::cout << "lowest frequency set to: " << radioSettings.active_bands.band_1.band.lowest_frequency << std::endl;
  RadioSettings_SteppableInt64SettingPb* centreFrequency = &radioSettingsPb.active_bands.band_1.pipeline_a.rf.centre_frequency;
  radioSettingsPb.has_active_bands = true;
  radioSettingsPb.active_bands.has_band_1 = true;
  radioSettingsPb.active_bands.band_1.has_pipeline_a = true;
  radioSettingsPb.active_bands.band_1.pipeline_a.has_rf = true;
  radioSettingsPb.active_bands.band_1.pipeline_a.rf.has_centre_frequency = true;
  centreFrequency->value = 14200000;
  centreFrequency->coarse_delta = 1000;
  centreFrequency->fine_delta = 100;
  centreFrequency->use_fine = false;

  RadioSettings radioSettings;


  // ResultCode rc = setMode(radioSettings, SplitBandId::One, PipelineId::A, Mode::Type::FMN);
  // ResultCode rc = radioSettings.setFocusMode(Mode::Type::FMN);
  NameString bandName = "160m";
  ResultCode rc = radioSettings.splitBands(true, bandName);

  return 0;
}