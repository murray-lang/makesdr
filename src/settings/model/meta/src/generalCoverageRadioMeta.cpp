#include "settings/model/meta/generalCoverageRadioMeta.h"


RadioSettings_RadioMetaPb generalCoverageRadioMeta = {
  .has_bands = true,
  .bands = {
    .categories_count = 7,
    .categories = {
      {
        "mw", "MW", true, {
          1,
          {
            {"MW", "MW", 526500, 160650, 810000, 1000, 9000, RadioSettings_ModeType_MODE_AMW}
          }
        }
      },
      {
        "hf", "HF", true, {
          9,
          {
            {"160m", "160m", 1800000, 1875000, 1840000, 100, 1000, RadioSettings_ModeType_MODE_LSB},
            {"80m", "80m", 3500000, 3800000, 3600000, 100, 1000, RadioSettings_ModeType_MODE_LSB},
            {"40m", "40m", 7000000, 7300000, 7100000, 100, 1000, RadioSettings_ModeType_MODE_LSB},
            {"30m", "30m", 10100000, 10150000, 10125000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"20m", "20m", 14000000, 14350000, 14200000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"17m", "17m", 18068000, 18168000, 18100000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"15m", "15m", 21000000, 21450000, 21200000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"12m", "12m", 24890000, 24990000, 24930000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"10m", "10m", 28000000, 29700000, 28400000, 100, 1000, RadioSettings_ModeType_MODE_USB}
          }
        }
      },
      {
        "vhf", "VHF", true, {
          4,
          {
            {"6m", "6m", 50000000, 54000000, 50110000, 8330, 25000, RadioSettings_ModeType_MODE_USB},
            {"2mSsb", "2m SSB", 144000000, 146000000, 144100000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"2mSimp", "2m Simp", 146400000, 146600000, 146500000, 12500, 25000, RadioSettings_ModeType_MODE_FMN},
            {"2mRpt", "2m Rpt", 146625000, 146700000, 148000000, 12500, 25000, RadioSettings_ModeType_MODE_FMN}
          }
        }
      },
      {
        "air", "Air", true, {
          1,
          {
              {"Air", "Air", 118000000, 137000000, 121500000, 100, 1000, RadioSettings_ModeType_MODE_AMW}
          }
        }
      },
      {
        "uhf", "UHF", true, {
          2,
          {
            {"70cm", "70cm", 420000000, 450000000, 432100000, 1000, 5000, RadioSettings_ModeType_MODE_USB},
            {"23cm", "23cm", 1240000000, 1300000000, 1296100000, 100, 1000, RadioSettings_ModeType_MODE_USB}
          }
        }
      },
      {
        "uw", "uW", true, {
          3,
          {
            {"13cm", "13cm", 2300000000, 2450000000, 2400100000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"9cm", "9cm", 3300000000, 3600000000, 3400100000, 100, 1000, RadioSettings_ModeType_MODE_USB},
            {"5cm", "5cm", 5650000000, 5850000000, 5760100000, 100, 1000, RadioSettings_ModeType_MODE_USB}
          }
        }
      },
      {
        "sw", "SW", true, {
          13,
          {
            {"120m", "120m", 2300000, 2495000, 2400000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"90m", "90m", 3200000, 3400000, 3300000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"75m", "75m", 3900000, 4000000, 3950000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"60m", "60m", 4750000, 5060000, 4900000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"49m", "49m", 5900000, 6200000, 6100000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"41m", "41m", 7200000, 7600000, 7400000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"31m", "31m", 9400000, 9900000, 9600000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"25m", "25m", 11600000, 12200000, 11900000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"22m", "22m", 13570000, 13870000, 13700000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"19m", "19m", 15100000, 15800000, 15400000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"16m", "16m", 17480000, 17900000, 17700000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"13m", "13m", 21450000, 21850000, 21650000, 1000, 5000, RadioSettings_ModeType_MODE_AMW},
            {"11m", "11m", 25600000, 26100000, 25850000, 1000, 5000, RadioSettings_ModeType_MODE_AMW}
          }
        }
      }
    }
  },
  .has_modes = true,
  .modes = {
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
  }
};
