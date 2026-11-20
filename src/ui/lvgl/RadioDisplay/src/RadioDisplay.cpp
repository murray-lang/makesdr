#include "ui/lvgl/RadioDisplay.h"
#include <lvgl_stm32h745i_disco_ui_project.h>
#include <widgets/panadapter/panadapter.h>
#include <widgets/frequency_display/frequency_display.h>

RadioDisplay::RadioDisplay()
  : m_radioUpdater(nullptr)
  , m_screen(nullptr)
  , m_panadapter(nullptr)
  , m_frequencyDisplay(nullptr)
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
  m_frequencyDisplay = lv_obj_find_by_name(m_screen, "frequency");
}

void
RadioDisplay::setFrequency(int32_t frequency)
{
  if (m_frequencyDisplay == nullptr) return;
  frequency_display_set_frequency(m_frequencyDisplay, frequency);
}

ResultCode
RadioDisplay::applyMessage(RxMeteringMessage* metering)
{
  (void)metering;
  return ResultCode::OK;
}

ResultCode
RadioDisplay::applyMessage(SpectrumMessage* spectrumMessage)
{
  if (spectrumMessage == nullptr || m_panadapter == nullptr) return ResultCode::OK;

  const makesdr_SpectrumPb_bins_t& bins = spectrumMessage->bins();

  // panadapter_set_bins() copies the bins into the chart
  const lv_image_dsc_t binsAsImage = { .data_size = bins.size, .data = bins.bytes };

  panadapter_set_bins(m_panadapter, &binsAsImage);
  return ResultCode::OK;
}
