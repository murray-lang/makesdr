#include "RadioControlClient.h"

#include "../../data/exampleRadioSettings.h"

#include "settings/model/core/RadioSettings.h"

#include "settings/model/lookup/radio/generalCoverageRadioLookup.h"

#include <settings/model/proto/ProtobufIo.h>
#include <stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h>

#include "../../data/exampleRadioSettings.h"
#include "settings/model/core/RadioPayloadBase.h"
#include "settings/model/proto/RadioPayloads.pb.h"

using RadioSettingsPb = makesdr_RadioSettingsPb;
using RadioSettingsPayloadPb = makesdr_RadioSettingsPayloadPb;

// RadioSettingsPb radioSettingsPb = makesdr_RadioSettingsPb_init_zero;
BandSettingsCache bandSettingsCache;

RadioControlClient::RadioControlClient()
  : m_radioSettings(exampleRadioSettingsPb, generalCoverageRadioLookup, bandSettingsCache)
{

}

ResultCode
RadioControlClient::applySettings(const RadioSettings& settings)
{
  return ResultCode::OK;
}
ResultCode
RadioControlClient::applyFieldUpdate(const FieldUpdate& update)
{
  ResultCode rc = m_radioSettings.applyFieldUpdate(update);
  if (rc == ResultCode::OK) {
    rc = notifySettings(m_radioSettings);
  }
  return rc;
}