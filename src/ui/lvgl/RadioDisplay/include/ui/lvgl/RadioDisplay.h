#pragma once
#include <ResultCode.h>
#include <settings/model/message/MessageSinkT.h>
#include <settings/model/radio/IRadioSettingsUpdater.h>
#include <settings/model/radio/RxMeteringMessage.h>
#include <settings/model/radio/iq/SpectrumMessage.h>
#include <settings/model/radio/iq/RxPipelineSettings.h>

#include "lvgl/lv_types.h"

//*****************************************************************************
// The LVGL front end. Everything here, including the sinks, must be called on
// the task that runs lv_timer_handler(). Messages are only valid for the
// duration of the call.
//*****************************************************************************
class RadioDisplay :
  public MessageSinkT<SpectrumMessage>,
  public MessageSinkT<RxMeteringMessage>
{
public:
  RadioDisplay();

  void initialise(IRadioSettingsUpdater* radioUpdater);

  void start();

  // 'state' is the client's accumulated settings; 'changes' says what changed
  // (gate on its has_* presence) and only lives for the duration of the call.
  template<typename RadioSettingsT>
  ResultCode handleRadioSettingsChanged(const RadioSettingsT& state, const RadioSettingsT& changes);

  ResultCode applyMessage(SpectrumMessage* spectrumMessage) override;
  ResultCode applyMessage(RxMeteringMessage* metering) override;

private:
  // Non-template LVGL side, so widget headers stay out of this header.
  void setFrequency(int32_t frequency);

  IRadioSettingsUpdater* m_radioUpdater;
  lv_obj_t * m_screen;
  lv_obj_t * m_panadapter;
  lv_obj_t * m_frequencyDisplay;

};

// Defined here rather than in the .cpp so it instantiates for whatever
// settings type the application uses.
template<typename RadioSettingsT>
ResultCode
RadioDisplay::handleRadioSettingsChanged(const RadioSettingsT& state, const RadioSettingsT& changes)
{
  (void)changes;
  using ActiveBandSettings = typename RadioSettingsT::ActiveBandSettings;
  using BandSettings = typename RadioSettingsT::BandSettings;

  if (state.hasActiveBands()) {
    const ActiveBandSettings* activeBandSettings = state.activeBands();
    if (activeBandSettings != nullptr && activeBandSettings->hasFocusBand()) {
      const BandSettings* bandSettings = activeBandSettings->focusBandSettings();
      if (bandSettings != nullptr && bandSettings->hasFocusPipeline()) {
        const RxPipelineSettings* pipelineSettings = bandSettings->focusPipeline();
        if (pipelineSettings != nullptr) {
          const PipelineRfSettings& rfSettings = pipelineSettings->base().rfSettings();
          if (rfSettings.hasFrequency()) {
            setFrequency(static_cast<int32_t>(rfSettings.frequency()));
          }
        }
      }
    }
  }

  return ResultCode::OK;
}
