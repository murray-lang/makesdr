#pragma once

#include <cstdint>
#include "settings/model/proto/RadioSettings.pb.h"

using ProtoBuffer = uint8_t[makesdr_ActiveBandSettingsPb_size]; // Maximum size. Could be any message.
