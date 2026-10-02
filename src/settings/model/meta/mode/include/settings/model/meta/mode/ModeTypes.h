#pragma once
#include "settings/model/proto/RadioSettings.pb.h"

#ifdef USE_ETL
#include <etl/string.h>
using ModeName = etl::string<sizeof(makesdr_ModePb::name)>;
#else
#include <string>
using ModeName = std::string;
#endif