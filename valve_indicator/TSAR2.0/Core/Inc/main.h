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
#include "stm32f0xx_hal.h"

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
#define Hall_Effect_Sensor_Nine_Pin GPIO_PIN_0
#define Hall_Effect_Sensor_Nine_GPIO_Port GPIOA
#define Hall_Effect_Sensor_One_Pin GPIO_PIN_1
#define Hall_Effect_Sensor_One_GPIO_Port GPIOA
#define Hall_Effect_Sensor_Two_Pin GPIO_PIN_2
#define Hall_Effect_Sensor_Two_GPIO_Port GPIOA
#define Hall_Effect_Sensor_Three_Pin GPIO_PIN_3
#define Hall_Effect_Sensor_Three_GPIO_Port GPIOA
#define Hall_Effect_Sensor_Four_Pin GPIO_PIN_4
#define Hall_Effect_Sensor_Four_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_0
#define Buzzer_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_1
#define SD_CS_GPIO_Port GPIOB
#define Hall_Effect_Sensor_Eight_Pin GPIO_PIN_8
#define Hall_Effect_Sensor_Eight_GPIO_Port GPIOA
#define Hall_Effect_Sensor_Seven_Pin GPIO_PIN_15
#define Hall_Effect_Sensor_Seven_GPIO_Port GPIOA
#define LED_Three_Pin GPIO_PIN_3
#define LED_Three_GPIO_Port GPIOB
#define LED_Two_Pin GPIO_PIN_4
#define LED_Two_GPIO_Port GPIOB
#define LED_One_Pin GPIO_PIN_5
#define LED_One_GPIO_Port GPIOB
#define Hall_Effect_Sensor_Six_Pin GPIO_PIN_6
#define Hall_Effect_Sensor_Six_GPIO_Port GPIOB
#define Hall_Effect_Sensor_Five_Pin GPIO_PIN_7
#define Hall_Effect_Sensor_Five_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define SD_SPI_HANDLE hspi1
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
