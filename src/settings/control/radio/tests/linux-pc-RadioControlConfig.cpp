#include "linux-pc-RadioControlConfig.h"

const char radioControlConfigJson[] = R"(

  {
        "sinks": [
          {
            "type": "usbcontrolsinks",
            "config": {
              "devices": [
                {
                  "type": "funcube",
                  "config": {
                  }
                },
                {
                  "type": "softrock",
                  "config": {
                  }
                }
              ]
            }
          },
          {
            "type": "qttransportout",
            "config": {
              "role": "radio",
              "target": "setting"
            }
          },
          {
            "type": "digitalOutputs",
            "config": {
              "outputs": [
                {
                  "type": "digitalOutput",
                  "config": {
                    "lines": [23],
                    "direction": "output",
                    "settingPath": "ptt"
                  }
                },
                {
                  "type": "bandSelector",
                  "config": {
                    "lines": [9, 10, 11, 17],
                    "direction": "output",
                    "defaultOut": 8,
                    "settingPath": "active_bands.focus_band.tx_pipeline.base.rf.center_frequency",
                    "bands": [
                      {
                        "fromFrequency": 1800000,
                        "toFrequency": 1875000,
                        "out": 0
                      },
                      {
                        "fromFrequency": 3500000,
                        "toFrequency": 3800000,
                        "out": 1
                      },
                      {
                        "fromFrequency": 7000000,
                        "toFrequency": 7300000,
                        "out": 2
                      },
                      {
                        "fromFrequency": 10100000,
                        "toFrequency": 10150000,
                        "out": 3
                      },
                      {
                        "fromFrequency": 14000000,
                        "toFrequency": 14350000,
                        "out": 4
                      },
                      {
                        "fromFrequency": 18068000,
                        "toFrequency": 18168000,
                        "out": 5
                      },
                      {
                        "fromFrequency": 21000000,
                        "toFrequency": 21450000,
                        "out": 6
                      },
                      {
                        "fromFrequency": 24890000,
                        "toFrequency": 24990000,
                        "out": 7
                      },
                      {
                        "fromFrequency": 28000000,
                        "toFrequency": 29700000,
                        "out": 8
                      }
                    ]
                  }
                }
              ]
            }
          }
        ],
        "sources": [
          {
            "type": "qttransportin",
            "config": {
              "role": "radio",
              "target": "setting"
            }
          },
          {
            "type": "digitalInputs",
            "config": {
              "inputs": [
                  {
					  "lines": [22],
					  "direction": "input",
					  "bias": "pull-up",
					  "debounce": true,
					  "settingPath": "ptt"

                  },
                  {
					  "lines": [8, 7],
					  "rotaryEncoder": true,
					  "direction": "input",
					  "bias": "pull-up",
					  "debounce": false,
					  "settingPath": "active_bands.focus_band.focus_pipeline.base.mode_request"

                  },
                  {
					  "lines": [13, 12],
					  "rotaryEncoder": true,
					  "direction": "input",
					  "bias": "pull-up",
					  "debounce": false,
					  "settingPath": "active_bands.focus_band.band_request"

                  },
                  {
					  "lines": [26, 16],
					  "rotaryEncoder": true,
					  "direction": "input",
					  "bias": "pull-up",
					  "debounce": false,
					  "settingPath": "active_bands.focus_band.focus_pipeline.base.rf.centre_frequency"

                  },
                  {
					  "lines": [15, 14],
					  "rotaryEncoder": true,
					  "direction": "input",
					  "bias": "none",
					  "debounce": false,
					  "settingPath": "active_bands.focus_band.focus_pipeline.base.rf.vfo"

                  }
              ]
            }
          }
        ]
  }


)";