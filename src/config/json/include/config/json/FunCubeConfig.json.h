//
// Created by murray on 15/9/25.
//

#pragma once
#include "ConfigBase.json.h"
#include "config/struct/FunCubeConfig.h"

namespace Config::FunCube
{
  extern ResultCode fromJson(const JsonVariantConst& json, Fields& fields);
}

// using FunCubeConfig = Config::FunCube::Fields;
