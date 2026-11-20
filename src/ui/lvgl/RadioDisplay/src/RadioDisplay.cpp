#include "ui/lvgl/RadioDisplay.h"
#include <lvgl_stm32h745i_disco_ui_project.h>
#include <widgets/panadapter/panadapter.h>

RadioDisplay::RadioDisplay()
  : m_radioUpdater(nullptr)
  , m_screen(nullptr)
  , m_panadapter(nullptr)
{

}

void
RadioDisplay::initialise(IRadioSettingsUpdater* radioUpdater)
{
  m_radioUpdater = radioUpdater;
}

void
RadioDisplay::start()
{
  lvgl_stm32h745i_disco_ui_project_init("");
  m_screen = radio_screen_create();
  lv_screen_load(m_screen);
  m_panadapter = lv_obj_find_by_name(m_screen, "panadapter");
}

template<typename RadioSettingsT>
ResultCode RadioDisplay::handleRadioSettingsChanged(RadioSettingsT* radioSettings)
{
  return ResultCode::OK;
}

void RadioDisplay::handleRxMetering(const RxMeteringMessage* metering)
{

}
template<typename RadioSettingsT>
void RadioDisplay::handleSpectrum(RadioSettingsT* radioSettings, const SpectrumMessage* spectrumMessage)
{
  if (spectrumMessage == nullptr || m_panadapter == nullptr) return;

  const makesdr_SpectrumPb_bins_t& bins = spectrumMessage->bins();

  const lv_image_dsc_t binsAsImage = { .data_size = bins.size, .data = bins.bytes };

  panadapter_set_bins(m_panadapter, &binsAsImage);

}