#include "settings/model/core/RadioSettings.h"

#include "settings/model/meta/generalCoverageRadioMeta.h"

#include <settings/model/proto/ProtobufIo.h>
#include "../../data/exampleRadioSettings.h"
#include "settings/model/core/RadioPayloadBase.h"
#include "settings/model/proto/RadioPayloads.pb.h"
#include "stm32h745i/app/setup/config.h"
#include "stm32h745i/drivers/bsp/disco/stm32h745i_discovery.h"

using RadioSettingsPb = makesdr_RadioSettingsPb;
using RadioSettingsPayloadPb = makesdr_RadioSettingsPayloadPb;


RadioSettingsPb radioSettingsPb = makesdr_RadioSettingsPb_init_zero;
BandSettingsCache bandSettingsCache;

int main()
{
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  MPU_Config();
  CPU_CACHE_Enable();

  /* Boot CM4 core */
  HAL_RCCEx_EnableBootCore(RCC_BOOT_C2);

  /* Immediately take HSEM before CM4 gets to STOP mode */
  __HAL_RCC_HSEM_CLK_ENABLE();
  HAL_HSEM_FastTake(HSEM_ID_0);  // Take it ASAP

  int32_t timeout = 0xFFFF;
  while((__HAL_RCC_GET_FLAG(RCC_FLAG_D2CKRDY) != RESET) && (timeout-- > 0));

  if ( timeout < 0 )
  {
    Error_Handler();
  }

  LOCK_HSEM(HSEM_ID_0);
  UNLOCK_HSEM(HSEM_ID_0);// This signals the CM4 to wake up

  ResultCode rc = ResultCode::OK;
  RadioSettings clientRadioSettings(generalCoverageRadioMeta, bandSettingsCache);

  bool isIndirect = false;
  AutoCompleteTrigger trigger;

  SettingPath bandPath;
  rc = RadioSettings::resolveDottedPath("active_bands.band_1.band_request", bandPath, &isIndirect, &trigger);
  if (rc != ResultCode::OK) {
    BSP_LED_On(LED_RED);
    while (1) {}
  }

  NameString band80 = "80m";
  SettingUpdate band80Update(bandPath, band80, SettingUpdate::VALUE, isIndirect, trigger);
  rc = clientRadioSettings.applySettingUpdate(band80Update);
  if (rc != ResultCode::OK) {
    BSP_LED_On(LED_RED);
    while (1) {}
  }

  // Serialise
  uint8_t buffer[makesdr_RadioSettingsPayloadPb_size];
  size_t written = 0;
  rc = clientRadioSettings.writeProtobuf(
    makesdr_RadioPayloadPurpose_PURPOSE_REPLACE,
    buffer,
    sizeof(buffer),
    &written
    );
  if (rc != ResultCode::OK) {
    BSP_LED_On(LED_RED);
    while (1) {}
  }

  RadioPayloadBase header;
  rc = header.readProtobuf(buffer, written);
  if (rc != ResultCode::OK) {
    BSP_LED_On(LED_RED);
    while (1) {}
  }

  makesdr_RadioPayloadType payloadType = header.type();
  if (payloadType != makesdr_RadioPayloadType_PAYLOAD_SETTINGS) {
    BSP_LED_On(LED_RED);
    while (1) {}
  }

  // Deserialise
  RadioSettings radioSettingsIn(generalCoverageRadioMeta, bandSettingsCache);
  rc = radioSettingsIn.readProtobuf(buffer, written);
  if (rc != ResultCode::OK) {
    BSP_LED_On(LED_RED);
    while (1) {}
  }

  RadioSettings radioSettings(generalCoverageRadioMeta, bandSettingsCache);
    switch (radioSettingsIn.purpose()) {
    case makesdr_RadioPayloadPurpose_PURPOSE_REPLACE:
      radioSettings.replace(radioSettingsIn.body(), true);
      radioSettings.autoComplete();
      break;
    case makesdr_RadioPayloadPurpose_PURPOSE_MERGE:
      radioSettings.merge(radioSettingsIn.body());
      break;

    default:
      rc = ResultCode::ERR_MESSAGE_PURPOSE_NOT_IMPLEMENTED;
      break;
    }
  if (rc != ResultCode::OK) {
    BSP_LED_On(LED_RED);
    while (1) {}
  }

  RadioSettings update(radioSettings.body(),generalCoverageRadioMeta, bandSettingsCache );
  radioSettings.setAllFieldsPresence(false);
  while (1) {}
}