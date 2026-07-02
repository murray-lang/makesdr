#pragma once

#include "settings/model/core/RadioSettings.h"

#include "settings/model/proto/RadioSettings.pb.h"
#include "settings/model/meta/generalCoverageRadioMeta.h"



#include <settings/model/proto/ProtobufIo.h>
#include "../../data/exampleRadioSettings.h"
using RadioSettingsPb = makesdr_RadioSettingsPb;

extern ResultCode indirectUpdate(RadioSettings& radioSettings);
