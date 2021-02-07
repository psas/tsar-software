// messages.h
// Portland State Aerospace Society
// TSAR Capstone 2020-2021
//
//	This is the header file for messages.c
//
//	References:
//
//
//  System Risk Factor = 0.33 (Catastrophic, Unlikely)
#ifndef __MESSAGES_H
#define __MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utilities.h"
#include <init.h>
#include "states_def.h"

uint32_t ProcessMessages(struct StateVars (*));
uint32_t OnTickStatusMessage(struct StateVars (*));
uint32_t SendStatusMessage(struct StateVars (*));
void Get_State_Disagree_Error_Msg(char (*),enum StateName , enum StateName );
void Get_Invalid_State_Error_Msg(char (*), enum StateName , enum StateName );
void Get_Valve_State_Status_Msg(char (*), struct StateVars (*) , uint32_t );
void UART_SendMessage(UART_HandleTypeDef *huart, char* message);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint32_t NominalValueCheck(struct StateVars *ctrl, char marionette[]);
void hexadecimalToDecimal(char* hexVal);
uint32_t ProcessCommands(struct StateVars * ctrl);
uint32_t getWords(char *base, char target[10][20]);

uint32_t status, cmd_flag, command, empty, nominal, retransition;
char buffer [256], Rx_indx, Rx_data[2], Rx_Buffer[256], Tx_Buffer[256] ;
volatile uint32_t RxTxFlags;

static uint32_t power_off     =  6835;    //hex: 1AB3    //off
static uint32_t stop          =  11086;   //hex: 2B4E    //stop
static uint32_t valve_check   =  19954;   //hex: 4DF2    //valve_check
static uint32_t lox_fill      =  24090;   //hex: 5E1A    //lox_fill
static uint32_t lox_chill     =  28555;   //hex: 6F8B    //lox_chill
static uint32_t lox_pre_fill  =  31250;   //hex: 7A12    //lox_prefill
static uint32_t pressurizing  =  35619;   //hex: 8B23    //pressurizing
static uint32_t safety        =  40005;   //hex: 9C45    //safety
static uint32_t ignition      =  43879;   //hex: AB67    //ignition
static uint32_t start_handler =  52617;   //hex: CD89    //start
static uint32_t stat          =  58866;   //hex: E5F2    //state status

#endif
