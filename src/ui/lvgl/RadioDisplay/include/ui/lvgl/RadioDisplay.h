#pragma once
#include <ResultCode.h>
#include <settings/model/radio/IRadioSettingsUpdater.h>
#include <settings/model/radio/RxMeteringMessage.h>
#include <settings/model/radio/iq/SpectrumMessage.h>

#include "lvgl/lv_types.h"

class RadioDisplay
{
public:
  RadioDisplay();

  void initialise(IRadioSettingsUpdater* radioUpdater);

  void start();

  template<typename RadioSettingsT>
  ResultCode handleRadioSettingsChanged(RadioSettingsT* radioSettings);
  void handleRxMetering(const RxMeteringMessage* metering);
  template<typename RadioSettingsT>
  void handleSpectrum(RadioSettingsT* radioSettings, const SpectrumMessage* spectrumMessage);

private:
  IRadioSettingsUpdater* m_radioUpdater;
  lv_obj_t * m_screen;
  lv_obj_t * m_panadapter;

};
