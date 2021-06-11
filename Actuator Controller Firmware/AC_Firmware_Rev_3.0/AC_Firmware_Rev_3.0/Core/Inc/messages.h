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

uint32_t status, cmd_flag, command, empty, nominal, retransition, inv_flag;
char buffer [256], Rx_indx, Rx_data[2], Rx_Buffer[100], Tx_Buffer[100] ;
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
//static uint32_t autocheck     =  46808;   //hex: B6D8    //valve checking (Automatic mode)

//Commands for the valve_check state (individual valves)
static uint32_t cycle_valves  =  57914;   //hex: E23A    //command to cycle valves one by one automatically
static uint32_t cycle_voff    =  28659;   //hex: 6FF3    // stop the automatic
static uint32_t IGON          =  18759;   //hex: 4947    // Igniter on
static uint32_t IGOFF         =  13477;   //hex: 34A5   // Igniter off
static uint32_t SOV1ON        =  24398;   //hex: 5F4E
static uint32_t SOV1OFF       =  36138;   //hex: 8D2A
static uint32_t SOV2ON        =  15850;   //hex: 3DEA
static uint32_t SOV2OFF       =  27528;   //hex: 6B88
static uint32_t SOV3ON        =  61867;   //hex: F1AB
static uint32_t SOV3OFF       =  49725;   //hex: C23D
static uint32_t SOV4ON        =  46061;   //hex: B3ED
static uint32_t SOV4OFF       =  36426;   //hex: 8E4A
static uint32_t SOV5ON        =  10983;   //hex: 2AE7
static uint32_t SOV5OFF       =  19052;   //hex: 4A6C
static uint32_t SOV6ON        =  26536;   //hex: 67A8
static uint32_t SOV6OFF       =  16098;   //hex: 3EE2
static uint32_t SOV7ON        =  21926;   //hex: 55A6
static uint32_t SOV7OFF       =  50615;   //hex: C5B7
static uint32_t SOV8ON        =  46072;   //hex: B3F8
static uint32_t SOV8OFF       =  35071;   //hex: 88FF
static uint32_t SOV9ON        =  47533;   //hex: B9AD
static uint32_t SOV9OFF       =  56639;   //hex: DD3F


#endif
