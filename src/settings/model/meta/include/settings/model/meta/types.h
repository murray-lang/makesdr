#pragma once
#include "settings/model/proto/RadioSettings.pb.h"

#ifdef USE_ETL
#include <etl/string.h>
using BandName = etl::string<sizeof(makesdr_BandPb::name)>;
using ModeName = etl::string<sizeof(makesdr_ModePb::name)>;
using BandCategoryName = etl::string<sizeof(makesdr_BandCategoryPb::name)>;
#else
#include <string>
using BandName = std::string<>;
using ModeName = std::string<>;
using BandCategoryName = std::string<>;
#endif