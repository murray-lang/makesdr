#include "settings/model/meta/basicModes.h"

RadioSettings_ModeListPb basicModes {
  .modes_count = 8,
    .modes = {
      {
        RadioSettings_ModeType_MODE_AMN,
        "AM",
        "AM (Narrow)",
        -3000,
        3000,
        0
      },
      {
        RadioSettings_ModeType_MODE_AMW,
        "AMW",
        "AM (Wide)",
        -9000,
        9000,
        0
      },
      {
        RadioSettings_ModeType_MODE_LSB,
        "LSB",
        "Lower Sideband",

        -4000,
        -200,
        0
      },
      {
        RadioSettings_ModeType_MODE_USB,
        "USB",
        "Upper Sideband",
        200,
        4000,
        0
      },
      {
        RadioSettings_ModeType_MODE_FMN,
        "FM",
        "FM (Narrow)",
        -5000,
        5000,
        0
      },
      {
        RadioSettings_ModeType_MODE_FMW,
        "FMW",
        "FM (Wide)",
        -15000,
        15000,
         0
      },
      {
        RadioSettings_ModeType_MODE_CWL,
        "CWL",
        "CW (Lower)",
        -1000,
        -50,
        -700
      },
      {
        RadioSettings_ModeType_MODE_CWU,
        "CWU",
        "CW (Upper)",
        50,
        1000,
        700
      }
    }
};