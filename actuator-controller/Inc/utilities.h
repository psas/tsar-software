// utilities.h
// Portland State Aerospace Society
// TSAR Capstone 2019-2020
//
// Change Log
// 	-Creation of Document 2/11/2020 [APJ]
//
//	This is the header file for utilities.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)

#ifndef UTILITIES_H
#define UTILITIES_H

#include "init.h"
#include "states_def.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx.h"
#include "stm32g0xx_hal_uart.h"
#include "stm32g0xx_hal_uart_ex.h"
#include "stm32g0xx_hal_def.h"
#include <stdio.h>
#include <string.h>

uint32_t VerifyState(int);
uint32_t StateConfiguration();
uint32_t ValveStateSetter(uint32_t);
uint32_t UART_SendMessage(UART_HandleTypeDef *,char (*));
uint32_t UART_RecieveMessage(UART_HandleTypeDef *);


#endif // UTILITIES_H
