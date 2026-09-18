#include <iostream>
#include <sstream>
#include <fstream>
#include <settings/control/radio/RadioControl.h>

#include "settings/model/core/RadioSettings.h"

#include "settings/model/lookup/radio/generalCoverageRadioLookup.h"

#include <settings/model/proto/ProtobufIo.h>
#include "../../data/exampleRadioSettings.h"
#include "settings/model/core/RadioPayloadBase.h"
#include "settings/model/proto/RadioPayloads.pb.h"

using RadioSettingsPb = makesdr_RadioSettingsPb;
using RadioSettingsPayloadPb = makesdr_RadioSettingsPayloadPb;


RadioSettingsPb radioSettingsPb = makesdr_RadioSettingsPb_init_zero;
BandSettingsCache bandSettingsCache;

int main()
{
  RadioControl radioControl;
  ResultCode rc = ResultCode::OK;
  RadioSettings clientRadioSettings(generalCoverageRadioLookup, bandSettingsCache);

  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  FieldPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.band_1.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) return -1;

  NameString band80 = "80m";
  FieldUpdate band80Update(bandPath, band80, FieldUpdate::VALUE, isIndirect, trigger);
  rc = clientRadioSettings.applyFieldUpdate(band80Update);
  if (rc != ResultCode::OK) return -1;

  // Serialise
  uint8_t buffer[makesdr_RadioSettingsPayloadPb_size];
  size_t written = 0;
  rc = clientRadioSettings.writeProtobuf(
    makesdr_RadioPayloadPurpose_PURPOSE_REPLACE,
    buffer,
    sizeof(buffer),
    &written
    );
  if (rc != ResultCode::OK) return -1;

  RadioPayloadBase header;
  rc = header.readProtobuf(buffer, written);
  if (rc != ResultCode::OK) return -1;

  makesdr_RadioPayloadType payloadType = header.type();
  if (payloadType != makesdr_RadioPayloadType_PAYLOAD_SETTINGS) {
    return -1;
  }

  // Deserialise
  RadioSettings radioSettingsIn(generalCoverageRadioLookup, bandSettingsCache);
  rc = radioSettingsIn.readProtobuf(buffer, written);
  if (rc != ResultCode::OK) return -1;

  RadioSettings radioSettings(generalCoverageRadioLookup, bandSettingsCache);
    switch (radioSettingsIn.purpose()) {
    case makesdr_RadioPayloadPurpose_PURPOSE_REPLACE:
      radioSettings.replace(radioSettingsIn.body(), true);
      radioSettings.autoComplete();
      break;
    case makesdr_RadioPayloadPurpose_PURPOSE_MERGE:
      radioSettings.merge(radioSettingsIn.body());
      break;

    default:
      rc = ResultCode::ERR_MESSAGE_PURPOSE_NOT_IMPLEMENTED;
      break;
    }
  if (rc != ResultCode::OK) return -1;

  RadioSettings update(radioSettings.body(),generalCoverageRadioLookup, bandSettingsCache );
  radioSettings.setAllFieldsPresence(false);

  return 0;
}