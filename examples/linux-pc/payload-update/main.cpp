#include <iostream>
#include <sstream>
#include <fstream>

#include "settings/model/core/RadioSettings.h"

#include "settings/model/meta/generalCoverageRadioMeta.h"

#include <settings/model/proto/ProtobufIo.h>
#include "../../data/exampleRadioSettings.h"
#include "settings/model/proto/RadioPayloads.pb.h"

using RadioSettingsPb = RadioSettings_RadioSettingsPb;
using RadioSettingsPayloadPb = RadioSettings_RadioSettingsPayloadPb;


RadioSettingsPb radioSettingsPb = RadioSettings_RadioSettingsPb_init_zero;
BandSettingsCache bandSettingsCache;

RadioSettingsPayloadPb radioPayloadOut;
RadioSettingsPayloadPb radioPayloadIn;


int main()
{
  ResultCode rc = ResultCode::OK;
  RadioSettings clientRadioSettings(generalCoverageRadioMeta, bandSettingsCache);

  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  SettingPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.band_1.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band80 = "80m";
  SettingUpdate band80Update(bandPath, band80, SettingUpdate::VALUE, isIndirect, trigger);
  rc = clientRadioSettings.applySettingUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  radioPayloadOut.payloadType = RadioSettings_RadioPayloadType_PAYLOAD_SETTINGS;
  radioPayloadOut.purpose = RadioSettings_RadioPayloadPurpose_PURPOSE_REPLACE;
  // radioPayloadOut.settings = exampleRadioSettingsPb;
  radioPayloadOut.settings = clientRadioSettings.raw();
  radioPayloadOut.has_settings = true;

  // Serialise
  uint8_t buffer[RadioSettings_RadioSettingsPayloadPb_size];
  size_t written = 0;
  rc = ProtobufIo::writeProtobuf<RadioSettingsPayloadPb>(
    buffer,
    RadioSettings_RadioSettingsPayloadPb_size,
    &written,
    &RadioSettings_RadioSettingsPayloadPb_msg,
    radioPayloadOut
    );
  if (rc != ResultCode::OK) return -1;

  // Deserialise
  rc = ProtobufIo::readProtobuf<RadioSettingsPayloadPb>(
    buffer,
    written,
    &RadioSettings_RadioSettingsPayloadPb_msg,
    RadioSettings_RadioSettingsPayloadPb_init_zero,
    radioPayloadIn
    );
  if (rc != ResultCode::OK) return -1;

  RadioSettings radioSettings(generalCoverageRadioMeta, bandSettingsCache);
  if (radioPayloadIn.payloadType == RadioSettings_RadioPayloadType_PAYLOAD_SETTINGS) {
    switch (radioPayloadIn.purpose) {
    case RadioSettings_RadioPayloadPurpose_PURPOSE_REPLACE:
      radioSettings.replace(radioPayloadIn.settings, true);
      break;
    case RadioSettings_RadioPayloadPurpose_PURPOSE_MERGE:
      radioSettings.merge(radioPayloadIn.settings);
      break;

    default:
      rc = ResultCode::ERR_MESSAGE_PURPOSE_NOT_IMPLEMENTED;
      break;
    }
  }
  if (rc != ResultCode::OK) return -1;

  RadioSettings update(radioSettings.raw(),generalCoverageRadioMeta, bandSettingsCache );
  radioSettings.setAllFieldsPresence(false);

  return 0;
}