/**
  ******************************************************************************
  * @file    usbh_platform.cpp
  * @brief   USB Host HAL callbacks and low-level functions for STM32H745I
  ******************************************************************************
  */

#include "usbh_core.h"
#include "sys/stm32h745i/drivers/hal/stm32h7xx_hal.h"

// External HCD (Host Controller Driver) handle - must be defined in main application
extern HCD_HandleTypeDef hhcd;

/**
  * @brief  Initializes Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{
    // Set LL driver parameters
    hhcd.Instance = USB_OTG_HS; // or USB_OTG_FS depending on your configuration
    hhcd.Init.Host_channels = 16;
    hhcd.Init.speed = HCD_SPEED_HIGH;
    hhcd.Init.dma_enable = ENABLE;
    hhcd.Init.phy_itface = USB_OTG_HS_EMBEDDED_PHY; // or USB_OTG_ULPI_PHY
    hhcd.Init.Sof_enable = DISABLE;
    hhcd.Init.low_power_enable = DISABLE;
    hhcd.Init.vbus_sensing_enable = ENABLE;
    hhcd.Init.use_external_vbus = DISABLE;

    // Link the driver to the stack
    hhcd.pData = phost;
    phost->pData = &hhcd;

    // Initialize LL Driver
    if (HAL_HCD_Init(&hhcd) != HAL_OK)
    {
        return USBH_FAIL;
    }

    USBH_LL_SetTimer(phost, HAL_GetTick());

    return USBH_OK;
}

/**
  * @brief  De-Initializes Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
    HAL_StatusTypeDef hal_status = HAL_HCD_DeInit(phost->pData);
    return (hal_status == HAL_OK) ? USBH_OK : USBH_FAIL;
}

/**
  * @brief  Starts the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
    HAL_StatusTypeDef hal_status = HAL_HCD_Start(phost->pData);
    return (hal_status == HAL_OK) ? USBH_OK : USBH_FAIL;
}

/**
  * @brief  Stops the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
    HAL_StatusTypeDef hal_status = HAL_HCD_Stop(phost->pData);
    return (hal_status == HAL_OK) ? USBH_OK : USBH_FAIL;
}

/**
  * @brief  Returns the USB Host Speed from the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Speeds
  */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
    USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

    switch (HAL_HCD_GetCurrentSpeed(phost->pData))
    {
    case USB_OTG_SPEED_HIGH:
        speed = USBH_SPEED_HIGH;
        break;

    case USB_OTG_SPEED_FULL:
        speed = USBH_SPEED_FULL;
        break;

    case USB_OTG_SPEED_LOW:
        speed = USBH_SPEED_LOW;
        break;

    default:
        speed = USBH_SPEED_FULL;
        break;
    }

    return speed;
}

/**
  * @brief  Resets the Host Port of the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
    HAL_StatusTypeDef hal_status = HAL_HCD_ResetPort(phost->pData);
    return (hal_status == HAL_OK) ? USBH_OK : USBH_FAIL;
}

/**
  * @brief  Returns the last transferred packet size.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval Packet Size
  */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

/**
  * @brief  Opens a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  epnum: Endpoint Number
  * @param  dev_address: Device USB address
  * @param  speed: Device Speed
  * @param  ep_type: Endpoint Type
  * @param  mps: Endpoint Max Packet Size
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe,
                                     uint8_t epnum, uint8_t dev_address,
                                     uint8_t speed, uint8_t ep_type, uint16_t mps)
{
    HAL_StatusTypeDef hal_status = HAL_HCD_HC_Init(phost->pData, pipe, epnum,
                                                     dev_address, speed, ep_type, mps);
    return (hal_status == HAL_OK) ? USBH_OK : USBH_FAIL;
}

/**
  * @brief  Closes a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HAL_StatusTypeDef hal_status = HAL_HCD_HC_Halt(phost->pData, pipe);
    return (hal_status == HAL_OK) ? USBH_OK : USBH_FAIL;
}

/**
  * @brief  Submits a new URB to the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  direction: Channel number
  * @param  ep_type: Endpoint Type
  * @param  token: Endpoint Type
  * @param  pbuff: pointer to URB data
  * @param  length: length of URB data
  * @param  do_ping: activate do ping protocol (for high speed only)
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe,
                                      uint8_t direction, uint8_t ep_type,
                                      uint8_t token, uint8_t *pbuff,
                                      uint16_t length, uint8_t do_ping)
{
    HAL_StatusTypeDef hal_status = HAL_HCD_HC_SubmitRequest(phost->pData, pipe,
                                                              direction, ep_type,
                                                              token, pbuff, length,
                                                              do_ping);
    return (hal_status == HAL_OK) ? USBH_OK : USBH_FAIL;
}

/**
  * @brief  Gets a URB state from the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval URB state
  */
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState(phost->pData, pipe);
}

/**
  * @brief  Drives VBUS.
  * @param  phost: Host handle
  * @param  state: VBUS state (0: OFF, 1: ON)
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
    // Implement VBUS control if available on your board
    // Example: GPIO control for VBUS enable
    /*
    if (state == 0)
    {
        // Disable VBUS
    }
    else
    {
        // Enable VBUS
    }
    */
    return USBH_OK;
}

/**
  * @brief  Sets toggle for a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  toggle: toggle (0/1)
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)
{
    HCD_HandleTypeDef *pHandle = (HCD_HandleTypeDef *)phost->pData;

    if (pHandle->hc[pipe].ep_is_in)
    {
        pHandle->hc[pipe].toggle_in = toggle;
    }
    else
    {
        pHandle->hc[pipe].toggle_out = toggle;
    }

    return USBH_OK;
}

/**
  * @brief  Returns the current toggle of a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval toggle (0/1)
  */
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    uint8_t toggle = 0U;
    HCD_HandleTypeDef *pHandle = (HCD_HandleTypeDef *)phost->pData;

    if (pHandle->hc[pipe].ep_is_in)
    {
        toggle = pHandle->hc[pipe].toggle_in;
    }
    else
    {
        toggle = pHandle->hc[pipe].toggle_out;
    }

    return toggle;
}

/**
  * @brief  Delay routine for the USB Host Library
  * @param  Delay: Delay in ms
  * @retval None
  */
void USBH_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}

/**
  * @brief  Timer tick update for USB Host time management
  * @param  phost: Host handle
  * @retval None
  */
void USBH_LL_IncTimer(USBH_HandleTypeDef *phost)
{
    USBH_LL_SetTimer(phost, HAL_GetTick());
}
