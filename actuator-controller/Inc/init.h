#ifndef INIT_H
#define INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utilities.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"

ADC_HandleTypeDef hadc1;

WWDG_HandleTypeDef hwwdg;

UART_HandleTypeDef hlpuart1;

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_ADC1_Init(void);
void MX_LPTIM1_Init(void);
void MX_WWDG_Init(void);

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

#ifdef __cplusplus
}
#endif

#endif // INIT_H
