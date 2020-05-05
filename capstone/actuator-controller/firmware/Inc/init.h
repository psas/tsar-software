#ifndef INIT_H
#define INIT_H

ADC_HandleTypeDef hadc1;

LPTIM_HandleTypeDef hlptim1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_LPTIM1_Init(void);

#endif
