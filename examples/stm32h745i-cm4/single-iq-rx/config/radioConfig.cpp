#include "radioConfig.h"
#include "digitalInputConfigs.h"

void createRadioConfig(Config::Radio::Fields& config)
{

  // FunCube sink
  // Config::FunCube::Fields funcube;
  // funcube.type = "funcube";
  // Config::Control::SinkConfigVariant funcubeVariant = funcube;
  // config.control.sinks.push_back(funcubeVariant);

  Config::UsbControlSinks::Fields usbControlSinks;
  usbControlSinks.type = "usbControlSinks";

  usbControlSinks.devices.push_back(Config::UsbControlSinks::UsbControlSinkConfigVariant{Config::SoftRock::Fields{}});
  Config::Control::SinkConfigVariant usbControlSinksVariant = usbControlSinks;
  config.control.sinks.push_back(usbControlSinks);


  // // Digital Outputs sink
  // Config::DigitalOutputs::Fields digitalOutputs;
  // digitalOutputs.type = "digitalOutputs";
  //
  // // PTT output
  // Config::DigitalOutput::Fields pttOutput;
  // pttOutput.type = "digitalOutput";
  // pttOutput.lines.push_back(23);
  // pttOutput.direction = "output";
  // pttOutput.settingPath = "ptt";
  // digitalOutputs.outputs.push_back(pttOutput);
  //
  // // Band selector output
  // Config::BandSelector::Fields bandSelector;
  // bandSelector.type = "bandSelector";
  // bandSelector.lines = {9, 10, 11, 17};
  // bandSelector.direction = "output";
  // bandSelector.defaultOut = 8;
  // bandSelector.settingPath = "active_bands.focus_band.tx_pipeline.base.rf.center_frequency";
  //
  // // Add bands
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{1800000, 1875000, 0});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{3500000, 3800000, 1});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{7000000, 7300000, 2});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{10100000, 10150000, 3});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{14000000, 14350000, 4});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{18068000, 18168000, 5});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{21000000, 21450000, 6});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{24890000, 24990000, 7});
  // bandSelector.bands.push_back(Config::BandSelector::Band::Fields{28000000, 29700000, 8});
  //
  // digitalOutputs.outputs.push_back(bandSelector);
  // Config::Control::SinkConfigVariant digitalOutputsVariant = move(digitalOutputs);
  // config.control.sinks.push_back(move(digitalOutputsVariant));

  // Digital Inputs source
  // Config::DigitalInputs::Fields digitalInputs;
  // digitalInputs.type = "digitalInputs";
  //
  // // PTT input
  // Config::DigitalInput::Fields pttInput;
  // pttInput.type = "digitalInput";
  // pttInput.lines.push_back(13);
  // pttInput.direction = "input";
  // pttInput.bias = "pull-up";
  // pttInput.debounce = false;
  // pttInput.activeHigh = true;
  // pttInput.rotaryEncoder = false;
  // pttInput.settingPath = "ptt";
  // digitalInputs.inputs.push_back(pttInput);

  // // Mode encoder
  // Config::DigitalInput::Fields modeEncoder;
  // modeEncoder.type = "digitalInput";
  // modeEncoder.lines = {4, 8};
  // modeEncoder.direction = "input";
  // modeEncoder.bias = "pull-up";
  // modeEncoder.debounce = false;
  // modeEncoder.activeHigh = false;
  // modeEncoder.rotaryEncoder = true;
  // modeEncoder.settingPath = "active_bands.focus_band.focus_pipeline.base.mode_request";
  // digitalInputs.inputs.push_back(modeEncoder);

  // // Band encoder
  // Config::DigitalInput::Fields bandEncoder;
  // bandEncoder.type = "digitalInput";
  // bandEncoder.lines = {13, 12};
  // bandEncoder.direction = "input";
  // bandEncoder.bias = "pull-up";
  // bandEncoder.debounce = false;
  // bandEncoder.activeHigh = false;
  // bandEncoder.rotaryEncoder = true;
  // bandEncoder.settingPath = "active_bands.focus_band.band_request";
  // digitalInputs.inputs.push_back(bandEncoder);
  //
  // Frequency encoder
  // Config::DigitalInput::Fields freqEncoder;
  // freqEncoder.type = "digitalInput";
  // freqEncoder.lines = {4, 8};
  // freqEncoder.direction = "input";
  // freqEncoder.bias = "pull-up";
  // freqEncoder.debounce = false;
  // freqEncoder.activeHigh = false;
  // freqEncoder.rotaryEncoder = true;
  // freqEncoder.settingPath = "active_bands.focus_band.focus_pipeline.base.rf.centre_frequency";
  // digitalInputs.inputs.push_back(freqEncoder);
  //
  // // VFO encoder
  // Config::DigitalInput::Fields vfoEncoder;
  // vfoEncoder.type = "digitalInput";
  // vfoEncoder.lines = {15, 14};
  // vfoEncoder.direction = "input";
  // vfoEncoder.bias = "none";
  // vfoEncoder.debounce = false;
  // vfoEncoder.activeHigh = false;
  // vfoEncoder.rotaryEncoder = true;
  // vfoEncoder.settingPath = "active_bands.focus_band.focus_pipeline.base.rf.vfo";
  // digitalInputs.inputs.push_back(vfoEncoder);

  Config::Control::SourceConfigVariant digitalInputsVariant = move(digitalInputsConfig);
  config.control.sources.push_back(move(digitalInputsVariant));

  Config::AdcIqSource::Fields adcIqSource = {
    Config::Alternative{ Config::AdcIqSource::type },
    32000
  };
  Config::IqIo::IqSourceConfigVariant iqSource = adcIqSource;
  // Speaker on receive, line-in on transmit (half duplex): the WM8994 on SAI2.
  Config::SaiAudio::Fields saiAudio = {
    Config::Alternative{ Config::SaiAudio::type },
    70, // outputVolume
    75  // inputVolume
  };
  Config::IqIo::AudioOutputConfigVariant audioOutput = saiAudio;
  Config::IqIo::Fields iqIo = {
    .iqSource = iqSource,
    .audioOutput = audioOutput
  };
  Config::IqReceiver::Fields iqReceiver = {
    .iqIo = iqIo
  };
  Config::Sdr::Fields sdr = {
    .receiver = iqReceiver
  };
  config.sdr = sdr;

  // Receiver configuration
  // TODO: Add receiver config initialization

  // Transmitter configuration
  // TODO: Add transmitter config initialization
}
