/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "init.h"

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
#define LOW_POWER_UART1_TX_Pin GPIO_PIN_1
#define LOW_POWER_UART1_TX_GPIO_Port GPIOC
#define GPIO13_Pin GPIO_PIN_13
#define GPIO13_GPIO_Port GPIOB
#define GPIO14_Pin GPIO_PIN_14
#define GPIO14_GPIO_Port GPIOB
#define GPIO15_Pin GPIO_PIN_15
#define GPIO15_GPIO_Port GPIOB
#define GPIO8_Pin GPIO_PIN_8
#define GPIO8_GPIO_Port GPIOA
#define GPIO9_FAST_Pin GPIO_PIN_9
#define GPIO9_FAST_GPIO_Port GPIOA
#define GPIO6_Pin GPIO_PIN_6
#define GPIO6_GPIO_Port GPIOC
#define GPIO7_Pin GPIO_PIN_7
#define GPIO7_GPIO_Port GPIOC
#define GPIO8D8_Pin GPIO_PIN_8
#define GPIO8D8_GPIO_Port GPIOD
#define GPIO9_Pin GPIO_PIN_9
#define GPIO9_GPIO_Port GPIOD
#define GPIO10_FAST_Pin GPIO_PIN_10
#define GPIO10_FAST_GPIO_Port GPIOA
#define GPIO2_Pin GPIO_PIN_2
#define GPIO2_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
