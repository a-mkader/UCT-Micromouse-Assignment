/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_gpio.h"

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
#define SW1_Pin LL_GPIO_PIN_6
#define SW1_GPIO_Port GPIOE
#define LED0_Pin LL_GPIO_PIN_13
#define LED0_GPIO_Port GPIOC
#define LED1_Pin LL_GPIO_PIN_14
#define LED1_GPIO_Port GPIOC
#define LED2_Pin LL_GPIO_PIN_15
#define LED2_GPIO_Port GPIOC
#define SW2_Pin LL_GPIO_PIN_2
#define SW2_GPIO_Port GPIOB
#define LED_FWD_LS_Pin LL_GPIO_PIN_8
#define LED_FWD_LS_GPIO_Port GPIOE
#define LED_DOWN_LS_Pin LL_GPIO_PIN_9
#define LED_DOWN_LS_GPIO_Port GPIOE
#define LED_RS_Pin LL_GPIO_PIN_11
#define LED_RS_GPIO_Port GPIOE
#define LED_LS_Pin LL_GPIO_PIN_12
#define LED_LS_GPIO_Port GPIOE
#define LED_MOT_RS_Pin LL_GPIO_PIN_13
#define LED_MOT_RS_GPIO_Port GPIOE
#define LED_DOWN_RS_Pin LL_GPIO_PIN_14
#define LED_DOWN_RS_GPIO_Port GPIOE
#define LED_FWD_RS_Pin LL_GPIO_PIN_15
#define LED_FWD_RS_GPIO_Port GPIOE
#define LED_MOT_LS_Pin LL_GPIO_PIN_12
#define LED_MOT_LS_GPIO_Port GPIOB
#define MOT_RIGHT_FWD_Pin LL_GPIO_PIN_6
#define MOT_RIGHT_FWD_GPIO_Port GPIOC
#define MOT_RIGHT_BWD_Pin LL_GPIO_PIN_7
#define MOT_RIGHT_BWD_GPIO_Port GPIOC
#define MOT_LEFT_FWD_Pin LL_GPIO_PIN_8
#define MOT_LEFT_FWD_GPIO_Port GPIOC
#define MOT_LEFT_BWD_Pin LL_GPIO_PIN_9
#define MOT_LEFT_BWD_GPIO_Port GPIOC
#define MOTOR_EN_Pin LL_GPIO_PIN_7
#define MOTOR_EN_GPIO_Port GPIOD
#define CTRL_LEDS_Pin LL_GPIO_PIN_3
#define CTRL_LEDS_GPIO_Port GPIOB
#define MPU6050_INT_Pin LL_GPIO_PIN_5
#define MPU6050_INT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
