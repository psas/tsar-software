
// init.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for init.c
//  init.c and init.h was created to avoid loosing sections of the code in case
//   of any changes to .ioc file.
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INIT_H
#define __INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "utilities.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void RxTxBuffer_Init(void);
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LOX_VENT_Pin GPIO_PIN_10
#define LOX_VENT_GPIO_Port GPIOB
#define LOX_PURGE_Pin GPIO_PIN_13
#define LOX_PURGE_GPIO_Port GPIOB
#define LOX_FILL_Pin GPIO_PIN_15
#define LOX_FILL_GPIO_Port GPIOB
#define FUEL_VENT_Pin GPIO_PIN_6
#define FUEL_VENT_GPIO_Port GPIOC
#define FUEL_PURGE_N2_Pin GPIO_PIN_7
#define FUEL_PURGE_N2_GPIO_Port GPIOC
#define LOX_MAIN_Pin GPIO_PIN_8
#define LOX_MAIN_GPIO_Port GPIOA
#define FUEL_MAIN_Pin GPIO_PIN_9
#define FUEL_MAIN_GPIO_Port GPIOA
#define IGNITION1_Pin GPIO_PIN_10
#define IGNITION1_GPIO_Port GPIOA
#define FUEL_PRESSURE_Pin GPIO_PIN_11
#define FUEL_PRESSURE_GPIO_Port GPIOA
#define LOX_PRESSURE_Pin GPIO_PIN_12
#define LOX_PRESSURE_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __INIT_H */


