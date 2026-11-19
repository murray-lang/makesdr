/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM6_Period 999
#define SDIO1_D2_Pin GPIO_PIN_10
#define SDIO1_D2_GPIO_Port GPIOC
#define Digital_Input_15_Pin GPIO_PIN_15
#define Digital_Input_15_GPIO_Port GPIOA
#define Digital_Output_4_Pin GPIO_PIN_4
#define Digital_Output_4_GPIO_Port GPIOB
#define DigitalInput_3_Pin GPIO_PIN_3
#define DigitalInput_3_GPIO_Port GPIOD
#define SDIO1_D3_Pin GPIO_PIN_11
#define SDIO1_D3_GPIO_Port GPIOC
#define Digital_Output_2_Pin GPIO_PIN_2
#define Digital_Output_2_GPIO_Port GPIOI
#define OSC32_OUT_Pin GPIO_PIN_15
#define OSC32_OUT_GPIO_Port GPIOC
#define OSC32_IN_Pin GPIO_PIN_14
#define OSC32_IN_GPIO_Port GPIOC
#define SDIO1_CK_Pin GPIO_PIN_12
#define SDIO1_CK_GPIO_Port GPIOC
#define SDIO1_D5_Pin GPIO_PIN_9
#define SDIO1_D5_GPIO_Port GPIOB
#define SDIO1_D4_Pin GPIO_PIN_8
#define SDIO1_D4_GPIO_Port GPIOB
#define SDIO1_CMD_Pin GPIO_PIN_2
#define SDIO1_CMD_GPIO_Port GPIOD
#define USB_OTG_FS_VBUS_Pin GPIO_PIN_9
#define USB_OTG_FS_VBUS_GPIO_Port GPIOA
#define Blue_Button_Pin GPIO_PIN_13
#define Blue_Button_GPIO_Port GPIOC
#define Digital_Output_8_Pin GPIO_PIN_8
#define Digital_Output_8_GPIO_Port GPIOI
#define Digital_Input_6_Pin GPIO_PIN_6
#define Digital_Input_6_GPIO_Port GPIOE
#define SDIO1_D0_Pin GPIO_PIN_8
#define SDIO1_D0_GPIO_Port GPIOC
#define SDIO1_D1_Pin GPIO_PIN_9
#define SDIO1_D1_GPIO_Port GPIOC
#define USB_OTG_FS2_P_Pin GPIO_PIN_12
#define USB_OTG_FS2_P_GPIO_Port GPIOA
#define USB_OTG_FS2_N_Pin GPIO_PIN_11
#define USB_OTG_FS2_N_GPIO_Port GPIOA
#define SDIO1_D7_Pin GPIO_PIN_7
#define SDIO1_D7_GPIO_Port GPIOC
#define SDIO1_D6_Pin GPIO_PIN_6
#define SDIO1_D6_GPIO_Port GPIOC
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define Digital_Input_1_Pin GPIO_PIN_1
#define Digital_Input_1_GPIO_Port GPIOK
#define Digital_Input_4_Pin GPIO_PIN_4
#define Digital_Input_4_GPIO_Port GPIOH
#define VCP_TX_Pin GPIO_PIN_10
#define VCP_TX_GPIO_Port GPIOB
#define VCP_RX_Pin GPIO_PIN_11
#define VCP_RX_GPIO_Port GPIOB
#define USB_OTG_Power_Pin GPIO_PIN_5
#define USB_OTG_Power_GPIO_Port GPIOA
#define Digital_Input_8_Pin GPIO_PIN_8
#define Digital_Input_8_GPIO_Port GPIOH

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
